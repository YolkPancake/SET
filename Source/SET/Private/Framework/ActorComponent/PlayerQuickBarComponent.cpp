// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "Net/UnrealNetwork.h"
#include "Object/ItemUseAction/ItemUseActionBase.h"
#include "Subsystem/UseActionManager.h"

UPlayerQuickBarComponent::UPlayerQuickBarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

bool UPlayerQuickBarComponent::HasTheTaskBeenCompleted()
{
	return bIsInventoryDataReady;
}

int UPlayerQuickBarComponent::GetCurrentQuickBarIndex() const
{
	return CurrentQBIndex;
}


void UPlayerQuickBarComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerQuickBar.OwnerStorable = this;
	if (GetOwner()->HasAuthority())
	{
		PlayerQuickBar.InitList(5);

		//TODO 反序列化存档数据
		bIsInventoryDataReady = true;
	}
}

void UPlayerQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPlayerQuickBarComponent, PlayerQuickBar, ELifetimeCondition::COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerQuickBarComponent, bIsInventoryDataReady, ELifetimeCondition::COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerQuickBarComponent, CurrentQBIndex, ELifetimeCondition::COND_OwnerOnly);
}

void UPlayerQuickBarComponent::NotifyInventoryItemChanged(const TArray<int32>& Indexs)
{
	// 权威端也会跑(用来补偿监听不会触发),由实现方自行拦截
	// 只有本地控制器才需要通知控件刷新
	APlayerController* PC = GetOwner<APlayerController>();
	if (!PC->IsLocalController()) return;
	// 因为拖拽和使用物品都会导致当前手持物品发生变化,走到这里
	// 先判断手持物品是否发生了变化,如果变化,再对比前后是否是同一个物品
	// 这样能够排除因使用导致数量发生变化而触发OnHeldItemChanged
	if (Indexs.Contains(CurrentQBIndex) && !CachedInvSlot.EqualTo(PlayerQuickBar.GetInventorySlotAt(CurrentQBIndex)))
	{
		CachedInvSlot = PlayerQuickBar.GetInventorySlotAt(CurrentQBIndex);
		if (CurrentUseAction.Get())
			CurrentUseAction->ResetUseAction();
		CurrentUseAction = nullptr;
		if (PlayerQuickBar.IsValidSlotAt(CurrentQBIndex))
		{
			UUseActionManager* UseActionManager = UUseActionManager::Get(this);
			CurrentUseAction = UseActionManager->GetItemUseAction(PC, PlayerQuickBar.GetItemTypeAt(CurrentQBIndex));
			if (CurrentUseAction.Get())
				CurrentUseAction->InitUseAction(PC);
		}
		OnHeldItemChanged.Broadcast();
	}
	OnInventoryItemChanged.Broadcast(Indexs);
}

FInventoryList& UPlayerQuickBarComponent::GetInventory()
{
	return PlayerQuickBar;
}

bool UPlayerQuickBarComponent::CanAcceptItem(const FGameplayTag& ItemType)
{
	return true;
}

void UPlayerQuickBarComponent::DispatchGameInput(const FGameplayTag& InKey, const FGameplayTag& InState)
{
	if (CurrentUseAction.Get())
		CurrentUseAction->HandleInputKey(InKey, InState);
}

int32 UPlayerQuickBarComponent::GetInventorySize() const
{
	return PlayerQuickBar.GetSize();
}

void UPlayerQuickBarComponent::Request_SwitchQuickBar(int NewIndex)
{
	// 输入是本地进到服务器的,本地通过复制拿到新的Index,先尝试本地拦截
	if (CurrentQBIndex == NewIndex) return;
	GetOwner()->HasAuthority() 
		? Authority_DoSwitchQuickBar(NewIndex)
		: Server_DoSwitchQuickBar(NewIndex);
}

void UPlayerQuickBarComponent::Authority_DoSwitchQuickBar(int NewIndex)
{
	CurrentQBIndex = NewIndex;
	OnRep_CurrentQBIndex();
}

void UPlayerQuickBarComponent::Server_DoSwitchQuickBar_Implementation(int NewIndex)
{
	Authority_DoSwitchQuickBar(NewIndex);
}

void UPlayerQuickBarComponent::OnRep_CurrentQBIndex()
{
	APlayerController* PC = GetOwner<APlayerController>();
	if (PC->IsLocalController())
	{
		CachedInvSlot = PlayerQuickBar.GetInventorySlotAt(CurrentQBIndex);
		if (CurrentUseAction.Get())
			CurrentUseAction->ResetUseAction();
		CurrentUseAction = nullptr;
		if (PlayerQuickBar.IsValidSlotAt(CurrentQBIndex))
		{
			UUseActionManager* UseActionManager = UUseActionManager::Get(this);
			CurrentUseAction = UseActionManager->GetItemUseAction(PC, PlayerQuickBar.GetItemTypeAt(CurrentQBIndex));
			if (CurrentUseAction.Get())
				CurrentUseAction->InitUseAction(PC);
		}
	}
	OnCurrentQuikcBarIndexChanged.Broadcast(CurrentQBIndex);
	OnHeldItemChanged.Broadcast();
}

void UPlayerQuickBarComponent::OnRep_bIsInventoryDataReady()
{
	CachedInvSlot = PlayerQuickBar.GetInventorySlotAt(CurrentQBIndex);
}


