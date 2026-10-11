// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SessionManager.h"
#include "Define/SETBaseDefine.h"

USessionManager* USessionManager::Get(UObject* WorldCtxObj)
{
	if (GEngine)
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldCtxObj, EGetWorldErrorMode::LogAndReturnNull))
			if (UGameInstance* Ins = World->GetGameInstance())
				return Ins->GetSubsystem<USessionManager>();
	return nullptr;
}

void USessionManager::ResetSessionManager()
{

}

void USessionManager::InitSessionContext()
{
	if (!SessionInterface.IsValid())
	{
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
	ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(0);
	UserId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
}

void USessionManager::CreateSession(const FSessionCreationContext & Ctx, FCreateSessionFailedDelegate CallBack)
{
	InitSessionContext();
	CreateSessionFailedCallBack = CallBack;
	if (FNamedOnlineSession* ExistSession = SessionInterface->GetNamedSession(NAME_GameSession))
	{
		CachedCreationCtx = Ctx;
		OnDestorySessionBeforeCreate_Handle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
			FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionManager::OnDestorySessionBeforeCreate_CallBack));
		SessionInterface->DestroySession(NAME_GameSession);
		return;
	}
	FOnlineSessionSettings  Settings;
	Settings.bIsLANMatch = true;
	Settings.bIsDedicated = false;
	Settings.bShouldAdvertise = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bAllowInvites = true;
	Settings.NumPublicConnections = Ctx.MaxPlayerNum;
	Settings.NumPrivateConnections = 0;
	Settings.Set(SETNamedKey::SETServerName, Ctx.ServerName);
	OnCreateSessionComplete_Handle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionManager::OnCreateSessionComplete_CallBack));
	SessionInterface->CreateSession(*UserId, NAME_GameSession, Settings);
}

void USessionManager::OnCreateSessionComplete_CallBack(FName SessionName, bool bIsSuccess)
{
	SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionComplete_Handle);
	if (bIsSuccess)
	{
		GetGameInstance()->GetWorld()->ServerTravel({ TEXT("/Game/Core/Map/GameMap?listen") }, true, false);
	}
	else
	{
		const FText& Msg = FText::FromString(TEXT("Session创建失败,请查看控制台信息"));
		CreateSessionFailedCallBack.ExecuteIfBound(Msg);
	}
	ClearCreateSessionData();
}

void USessionManager::ClearCreateSessionData()
{
	CreateSessionFailedCallBack = FCreateSessionFailedDelegate{};
	CachedCreationCtx = FSessionCreationContext{};
}

void USessionManager::OnDestorySessionBeforeCreate_CallBack(FName SessionName, bool bIsSuccess)
{
	SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestorySessionBeforeCreate_Handle);
	FOnlineSessionSettings  Settings;	
	Settings.bIsLANMatch = true;
	Settings.NumPublicConnections = CachedCreationCtx.MaxPlayerNum;
	Settings.Set(SETNamedKey::SETServerName, CachedCreationCtx.ServerName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OnCreateSessionComplete_Handle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionManager::OnCreateSessionComplete_CallBack));
	SessionInterface->CreateSession(*UserId, NAME_GameSession, Settings);
}

void USessionManager::FindSession(const FSessionFindContext& Context, FFindSessionCompleteDelegate CallBackDelegate)
{
	InitSessionContext();
	FindSessionCompleteDelegate = CallBackDelegate;
	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 50;
	SessionSearch->PingBucketSize = 300;
	OnFindSessionsComplete_Handle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionManager::OnFindSessionComplete_CallBack));
	SessionInterface->FindSessions(*UserId, SessionSearch.ToSharedRef());
}

void USessionManager::OnFindSessionComplete_CallBack(bool bIsSuccess)
{
	SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsComplete_Handle);
	TArray<FSessionInformationData> InfoData;
	if (bIsSuccess)
	{
		InfoData.Reserve(SessionSearch->SearchResults.Num());
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FSessionInformationData Data;
			Data.MaxPlayerNum = Result.Session.NumOpenPublicConnections;
			FString ServerName;
			Result.Session.SessionSettings.Get(SETNamedKey::SETServerName, ServerName);
			Data.ServerName = ServerName;
			InfoData.Add(Data);
		}
	}
	const FText Msg = bIsSuccess
		? FText::GetEmpty()
		: FText::FromString(TEXT("Session查找失败,请查看控制台信息"));
	FindSessionCompleteDelegate.ExecuteIfBound(bIsSuccess, Msg, InfoData);
	ClearFindSessionData();
}

void USessionManager::ClearFindSessionData()
{
	FindSessionCompleteDelegate = FFindSessionCompleteDelegate{};
}


void USessionManager::JoinSession(const FJoinSessionContext& Ctx, FJoinSessionFailedDelegate CallBackDelegate, APlayerController* PC)
{
	InitSessionContext();
	JoinSessionFailedDelegate = CallBackDelegate;
	TargetPC = PC;
	// TODO: checkf
	checkf(SessionSearch.IsValid(),
		TEXT("无效的SessionSearch,在尝试JoinSession前,请先SearchSession!"));
	checkf(SessionSearch->SearchResults.IsValidIndex(Ctx.SessionIndex),
		TEXT("%d并非是一个有效的SearchResultsIndex,可能的原因有没有find直接join,或者控件展示方index设置有问题,请检查!"),Ctx.SessionIndex);
	FOnlineSessionSearchResult Session = SessionSearch->SearchResults[Ctx.SessionIndex];
	OnJoinSessionComplete_Handle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionManager::OnJoinSessionComplete_CallBack));
	SessionInterface->JoinSession(*UserId, NAME_GameSession, Session);
}

void USessionManager::OnJoinSessionComplete_CallBack(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionComplete_Handle);
	FText Msg;
	bool bIsSuccess = false;
	switch (Result)
	{
		case EOnJoinSessionCompleteResult::Success:
			Msg = FText::FromString({ TEXT("加入房间成功,请开始加载游戏") });
			bIsSuccess = true;
			break;
		case EOnJoinSessionCompleteResult::SessionIsFull:
			Msg = FText::FromString({ TEXT("房间已满") });
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			Msg = FText::FromString({ TEXT("房间不存在") });
			break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			Msg = FText::FromString({ TEXT("无法解析地址") });
			break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			Msg = FText::FromString({ TEXT("已经在房间中了") });
			break;
		case EOnJoinSessionCompleteResult::UnknownError:
			Msg = FText::FromString({ TEXT("未知错误,请重试") });
			break;
		default:
			checkNoEntry();
			break;
	}
	if (bIsSuccess && TargetPC.IsValid())
	{
		FString ConnectString;
		SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectString);
		TargetPC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute, false);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"),*Msg.ToString());
		FText ErrorMsg = FText::FromString(FString::Printf(TEXT("加入Session失败,请查看控制台信息,当前PC是否有效:%d"), TargetPC.IsValid()));
		JoinSessionFailedDelegate.ExecuteIfBound(ErrorMsg);
	}
	ClearJoinSessionData();
}

void USessionManager::ClearJoinSessionData()
{
	JoinSessionFailedDelegate = FJoinSessionFailedDelegate{};
	TargetPC = nullptr;
}

void USessionManager::DestorySession(const FDestorySessionContext& Context, FDestoryCompleteDelegate CallBackDelegate)
{
	InitSessionContext();
	DestoryCompleteDelegate = CallBackDelegate;
	OnDestorySessionComplete_Handle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
		FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionManager::OnDestorySessionComplete_CallBack));
	SessionInterface->DestroySession(NAME_GameSession);
}

void USessionManager::OnDestorySessionComplete_CallBack(FName SessionName, bool bIsSuccess)
{
	const FText Msg = FText::FromString(FString::Printf(TEXT("尝试摧毁Sesison,结果:%d"), bIsSuccess));
	DestoryCompleteDelegate.ExecuteIfBound(bIsSuccess, Msg);
	ClearDestorySessionData();
}

void USessionManager::ClearDestorySessionData()
{
	DestoryCompleteDelegate = FDestoryCompleteDelegate{};
}


