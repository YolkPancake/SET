// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/TaskCoordinator.h"

#include "Subsystem/LoadTaskCollector.h"
#include "Subsystem/LoadingScreenManager.h"

#include "Subsystem/SessionManager.h"


UTaskCoordinator* UTaskCoordinator::Get(const UObject* WorldCtxObj)
{
	if (GEngine)
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldCtxObj, EGetWorldErrorMode::LogAndReturnNull))
			if (UGameInstance* GI = World->GetGameInstance())
				return GI->GetSubsystem<UTaskCoordinator>();
	UE_LOG(LogTemp, Error, TEXT("无法通过%s获取UTaskCoordinator"),*WorldCtxObj->GetClass()->GetName());
	return nullptr;
}

bool UTaskCoordinator::ShouldCreateSubsystem(UObject* Outer) const
{
	UGameInstance* GI = Cast<UGameInstance>(Outer);
	return !GI->IsDedicatedServerInstance();
}

void UTaskCoordinator::RunLoadDataAndNotifyToStartGameTask(APlayerController* PC, FOnGameStartedDelegat CallBack)
{
	ULoadTaskCollector* TaskCollector = GetGameInstance()->GetSubsystem<ULoadTaskCollector>();
	OnGameStartedDelegat = CallBack;
	FOnLoadDataTaskCompleteDelegat OnLoadDataCompleteCallBack;
	OnLoadDataCompleteCallBack.BindUObject(this, &UTaskCoordinator::OnLoadDataComplelt_CallBack);
	TaskCollector->RunLoadDataTask(PC, OnLoadDataCompleteCallBack);
}

void UTaskCoordinator::OnLoadDataComplelt_CallBack(bool bIsSuccess, const FText& Msg)
{
	ULoadingScreenManager* LoadingScreenManager = GetGameInstance()->GetSubsystem<ULoadingScreenManager>();
	LoadingScreenManager->RemoveLoadingScreen();
	if (bIsSuccess)
	{
		OnGameStartedDelegat.ExecuteIfBound(bIsSuccess, Msg);
		OnGameStartedDelegat = FOnGameStartedDelegat{};
	}
	else
	{
		// 游戏失败信息,先打印,暂定强制开局
		UE_LOG(LogTemp, Error, TEXT("游戏开局加载失败,将会强制开局,请擦看日志信息,本次失败信息:%s"), *Msg.ToString());
		OnGameStartedDelegat.ExecuteIfBound(bIsSuccess, Msg);
		OnGameStartedDelegat = FOnGameStartedDelegat{};
	}
}

