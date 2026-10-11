// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Controller/GameController.h"

#include "GameFramework/PlayerState.h"

#include "Subsystem/TaskCoordinator.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystem/WidgetManager.h"

#include "Framework/ActorComponent/RPCComponent.h"
#include "Framework/ActorComponent/PlayerBagComponent.h"
#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "Framework/ActorComponent/ItemTransferComponent.h"
#include "Framework/Character/GameCharacter.h"


#include "Framework/Actor/ContainerActor/ContainerActor.h"

AGameController::AGameController()
{
	PrimaryActorTick.bCanEverTick = true;

	BagComp = CreateDefaultSubobject<UPlayerBagComponent>(TEXT("BagComp"));
	QuickBarComp = CreateDefaultSubobject<UPlayerQuickBarComponent>(TEXT("QuickBarComp"));
	RPCComp = CreateDefaultSubobject<URPCComponent>(TEXT("RPCComp"));
	ItemTransferComp = CreateDefaultSubobject<UItemTransferComponent>(TEXT("ItemTransferComp"));
}

UPlayerBagComponent* AGameController::GetBagComponent() const
{
	return BagComp;
}

UPlayerQuickBarComponent* AGameController::GetQuickBarComponent() const
{
	return QuickBarComp;
}

UItemTransferComponent* AGameController::GetItemTransferComponent() const
{
	return ItemTransferComp;
}

URPCComponent* AGameController::GetRPCComponent() const
{
	return RPCComp;
}

void AGameController::BeginPlay()
{
	Super::BeginPlay();

	// Controller准备完毕后,启动一个0.5的定时器,轮询Character和PlayerState
	// 如果是房主,进入房间,三者的BeginPlay是延迟统一调用
	// 权威上的其他玩家中途加入,三者是边造边跑BeginPlay
	// 以GetWorld()->HasBegunPlay();为准,在之前,延迟调用,之后边造边调
	// 所以在Controller中轮询,等两者都好了之后,Controller取消定时,统一通知
	// 即使是房主进入,在GetWorld()->HasBegunPlay();也是正确的
	GetWorld()->GetTimerManager().SetTimer(
		CheckPlayerReady_THandle, this, &AGameController::DoCheckPlayerReady, 0.5f, true
	);
}

void AGameController::OnGameStarted_CallBack(bool bIsSuccess, const FText& Msg)
{
	// 本地客户端游戏开始的真正起点,通知Pawn和PS游戏开始,此时接输入,推游戏主HUD
	if (!bIsSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("OnGameStarted_CallBack返回false,游戏数据加载出现问题,无法开局,请查看控制台信息,额外附件信息:%s"),*Msg.ToString());
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(IMC_CA, 0);
	}

	UWidgetManager* WidgetManager = UWidgetManager::Get(this);
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::HUD::MainHUD);

	AGameCharacter* GameCA = GetPawn<AGameCharacter>();
	GameCA->HandleGameStarted();
}

void AGameController::DoCheckPlayerReady()
{
	APawn* PA = GetPawn();
	APlayerState* PS = PlayerState;
	bool bPA = IsValid(PA);
	bool bPS = IsValid(PS);
	bool bIsPAReady = false;
	bool bIsPSReady = false;
	if (bPA)
		bIsPAReady = IsValid(PA->GetController()) && IsValid(PA->GetPlayerState());
	if (bPS)
		bIsPSReady = IsValid(PS->GetPawn()) && IsValid(PS->GetOwner());
	if (bPA && bPS && bIsPAReady && bIsPSReady)
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckPlayerReady_THandle);
		AGameCharacter* GameCA = Cast<AGameCharacter>(PA);
		GameCA->HandlePlayerReady();

		// 由于这个逻辑同时跑在权威和客户端,客户端到这里也是三者都准备好了,从这里启动加载Task
		if (IsLocalController())
		{
			UTaskCoordinator* TaskCoordinator = UTaskCoordinator::Get(this);
			FOnGameStartedDelegat OnGameStartCallBack;
			OnGameStartCallBack.BindUObject(this, &AGameController::OnGameStarted_CallBack);
			TaskCoordinator->RunLoadDataAndNotifyToStartGameTask(this, OnGameStartCallBack);
		}
	}
}
