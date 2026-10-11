// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ActorComponent/PlayerBagComponent.h"
#include "Net/UnrealNetwork.h"
#include "Define/Struct/SETInventoryDataDefine.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"

UPlayerBagComponent::UPlayerBagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);


}

bool UPlayerBagComponent::HasTheTaskBeenCompleted()
{
	return bIsInventoryDataReady;
}

int UPlayerBagComponent::Authority_DoAddItem(const FInventorySlot& Slot)
{
	return Authority_DoAddItem(Slot.GetItemData());
}

int UPlayerBagComponent::Authority_DoAddItem(const FInventoryItemData& ItemData)
{
	checkf(GetOwner()->HasAuthority(), TEXT("调用Request_AddItem但并非权威环境!因为函数有返回,请确保在权威环境调用!"));
	const int32 TotalNum = ItemData.GetNum();
	if (!ItemData.IsValid() || TotalNum <= 0)
		return TotalNum;
	UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
	UItemDefinition* ItemDef = DefTable->GetItemDefinition(ItemData.ItemType);
	const int32 MaxStack = FMath::Max(1, ItemDef->MaxStack);
	int32 Remaining = TotalNum;
	if (MaxStack > 1)
	{
		TArray<int32> EmptyIndices;
		for (int32 i = 0; i < PlayerBag.GetSize() && Remaining > 0; ++i)
		{
			const FInventorySlot& InvSlot = PlayerBag.GetInventorySlotAt(i);
			if (!InvSlot.IsValidSlot())
			{
				EmptyIndices.Add(i);
				continue;
			}
			if (InvSlot.GetItemType() != ItemData.ItemType) continue;
			if (InvSlot.GetNum() == MaxStack) continue;
			const int32 Put = FMath::Min(MaxStack - InvSlot.GetNum(), Remaining);
			PlayerBag.SetItemNumAt(i,InvSlot.GetNum() + Put);
			Remaining -= Put;
		}
		for (const int32 Index : EmptyIndices)
		{
			if (Remaining <= 0) break;
			const int32 Put = FMath::Min(MaxStack, Remaining);
			PlayerBag.SetSlotAt(ItemData, Index);
			PlayerBag.SetItemNumAt(Index, Put);
			Remaining -= Put;
		}

		return Remaining;
	}
	checkf(TotalNum == 1, TEXT("非可堆叠物品应该一次只放一个,而不是一个槽位放置多个物品!"));
	for (int i = 0; i < PlayerBag.GetSize() && Remaining >0; i++)
	{
		if (PlayerBag.IsValidSlotAt(i)) continue;
		PlayerBag.SetSlotAt(ItemData, i);
		Remaining = 0;
	}
	return Remaining;
}

void UPlayerBagComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerBag.OwnerStorable = this;
	if (GetOwner()->HasAuthority())
	{
		PlayerBag.InitList(30);

		//TODO 反序列化存档数据
		bIsInventoryDataReady = true;
	}

	UE_LOG(LogTemp, Error, TEXT("Clinet: Has Authority %d"), GetOwner()->HasAuthority());
}

void UPlayerBagComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UPlayerBagComponent, PlayerBag, ELifetimeCondition::COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerBagComponent, bIsInventoryDataReady, ELifetimeCondition::COND_OwnerOnly);


}

void UPlayerBagComponent::NotifyInventoryItemChanged(const TArray<int32>& Indexs)
{
	// 权威端也会跑(用来补偿监听不会触发),由实现方自行拦截
	// 只有本地控制器才需要通知控件刷新
	if (!Cast<APlayerController>(GetOwner())->IsLocalController())
		return;
	OnInventoryItemChanged.Broadcast(Indexs);
}

FInventoryList& UPlayerBagComponent::GetInventory()
{
	return PlayerBag;
}

bool UPlayerBagComponent::CanAcceptItem(const FGameplayTag& ItemType)
{
	return true;
}

int UPlayerBagComponent::GetInventorySize() const
{
	return PlayerBag.GetSize();
}

int UPlayerBagComponent::Request_AddItem(const FInventorySlot& Slot)
{
	checkf(GetOwner()->HasAuthority(), TEXT("调用Request_AddItem但并非权威环境!因为函数有返回,请确保在权威环境调用!"));
	return Authority_DoAddItem(Slot);
}

int UPlayerBagComponent::Request_AddItem(const FInventoryItemData& ItemData)
{
	checkf(GetOwner()->HasAuthority(), TEXT("调用Request_AddItem但并非权威环境!因为函数有返回,请确保在权威环境调用!"));
	return Authority_DoAddItem(ItemData);
}

void UPlayerBagComponent::OnRep_bIsInventoryDataReady()
{


}



