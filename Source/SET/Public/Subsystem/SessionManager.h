// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Subsystem/SessionManager.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Define/Struct/SessionInfoContext.h"
#include "SessionManager.generated.h"






DECLARE_DELEGATE_OneParam(FCreateSessionFailedDelegate, const FText&);
DECLARE_DELEGATE_ThreeParams(FFindSessionCompleteDelegate,bool ,const FText&,const TArray<FSessionInformationData>&);
DECLARE_DELEGATE_OneParam(FJoinSessionFailedDelegate, const FText& Msg);
DECLARE_DELEGATE_TwoParams(FDestoryCompleteDelegate,bool ,const FText& Msg);

UCLASS()
class SET_API USessionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	static USessionManager* Get(UObject* WorldCtxObj);
	void ResetSessionManager();

protected:
	void InitSessionContext();


protected:
	TSharedPtr<const FUniqueNetId> UserId;

	IOnlineSessionPtr SessionInterface;





	// 创建对局
public:
	void CreateSession(const FSessionCreationContext& Ctx, FCreateSessionFailedDelegate CallBack);
protected:
	void OnCreateSessionComplete_CallBack(FName SessionName, bool bIsSuccess);
	void ClearCreateSessionData();

	FCreateSessionFailedDelegate CreateSessionFailedCallBack;
	FDelegateHandle OnCreateSessionComplete_Handle;
	// 如果已经存在,先销毁,再创建
	FSessionCreationContext CachedCreationCtx;
	FDelegateHandle OnDestorySessionBeforeCreate_Handle;
	void OnDestorySessionBeforeCreate_CallBack(FName SessionName, bool bIsSuccess);


	// 查找对局
public:
	void FindSession(const FSessionFindContext& Context, FFindSessionCompleteDelegate CallBackDelegate);

protected:
	void OnFindSessionComplete_CallBack(bool bIsSuccess);
	void ClearFindSessionData();

	FFindSessionCompleteDelegate FindSessionCompleteDelegate;
	FDelegateHandle OnFindSessionsComplete_Handle;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;



	//加入对局
public:
	void JoinSession(const FJoinSessionContext& Ctx, FJoinSessionFailedDelegate CallBackDelegate,APlayerController* PC);
protected:
	void OnJoinSessionComplete_CallBack(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void ClearJoinSessionData();
	FJoinSessionFailedDelegate JoinSessionFailedDelegate;
	FDelegateHandle OnJoinSessionComplete_Handle;
	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> TargetPC;


	//摧毁对局
public:
	void DestorySession(const FDestorySessionContext& Context, FDestoryCompleteDelegate CallBackDelegate);
protected:
	void OnDestorySessionComplete_CallBack(FName SessionName, bool bIsSuccess);
	void ClearDestorySessionData();
	FDestoryCompleteDelegate DestoryCompleteDelegate;
	FDelegateHandle OnDestorySessionComplete_Handle;
};
