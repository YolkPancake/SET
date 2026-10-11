// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ActorComponent/ItemTransferComponent.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinitionTable.h"
#include "DataAsset/ItemDefinition.h"


UItemTransferComponent::UItemTransferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UItemTransferComponent::Request_TransferItem(const TScriptInterface<IStorable>& SourceStorable, const int32 SourceIndex, const TScriptInterface<IStorable>& TargetStorable, const int32 TargetIndex)
{
	GetOwner()->HasAuthority()
		? Authority_TransferItem(SourceStorable, SourceIndex, TargetStorable, TargetIndex)
		: Server_TransferItem(SourceStorable, SourceIndex, TargetStorable, TargetIndex);
}

void UItemTransferComponent::Server_TransferItem_Implementation(const TScriptInterface<IStorable>& SourceStorable, const int32 SourceIndex, const TScriptInterface<IStorable>& TargetStorable, const int32 TargetIndex)
{
	Authority_TransferItem(SourceStorable, SourceIndex, TargetStorable, TargetIndex);
}

void UItemTransferComponent::Authority_TransferItem(const TScriptInterface<IStorable>& SourceStorable, const int32 SourceIndex, const TScriptInterface<IStorable>& TargetStorable, const int32 TargetIndex)
{
	if (!SourceStorable || !TargetStorable) return;
	if (SourceStorable == TargetStorable && SourceIndex == TargetIndex) return;
	FInventoryList& SourceInvList = SourceStorable->GetInventory();
	FInventoryList& TargetInvList = TargetStorable->GetInventory();
	const FInventorySlot SourceSlot = SourceInvList.GetInventorySlotAt(SourceIndex);
	const FInventorySlot TargetSlot = TargetInvList.GetInventorySlotAt(TargetIndex);
	if (!SourceStorable->CanAcceptItem(TargetSlot.GetItemType()) || !TargetStorable->CanAcceptItem(SourceSlot.GetItemType())) return;
	SourceStorable->PreTransferItem(SourceIndex);
	TargetStorable->PreTransferItem(TargetIndex);
	if (CanSwapItem(SourceSlot, TargetSlot))
	{
		SourceInvList.SetSlotAt(TargetSlot, SourceIndex);
		TargetInvList.SetSlotAt(SourceSlot, TargetIndex);
	}
	else
	{
		UItemDefinitionTable* ItemDefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
		UItemDefinition* SourceItemDef = ItemDefTable->GetItemDefinition(SourceSlot.GetItemType());
		UItemDefinition* TargetItemDef = ItemDefTable->GetItemDefinition(TargetSlot.GetItemType());
		const int Remaining = TargetItemDef->MaxStack - TargetSlot.GetNum();
		if (Remaining >= SourceSlot.GetNum())
		{
			TargetInvList.SetItemNumAt(TargetIndex, TargetSlot.GetNum() + SourceSlot.GetNum());
			SourceInvList.ClearSlotAt(SourceIndex);
		}
		else
		{
			TargetInvList.SetItemNumAt(TargetIndex, TargetItemDef->MaxStack);
			SourceInvList.SetItemNumAt(SourceIndex, SourceSlot.GetNum() - Remaining);
		}
	}
	SourceStorable->PostTransferItem(SourceIndex);
	TargetStorable->PostTransferItem(TargetIndex);
}

bool UItemTransferComponent::CanSwapItem(const FInventorySlot& SourceSlot, const FInventorySlot& TargetSlot)
{
	if (TargetSlot.GetItemType() == SETGameplayTag::ItemType::None)
		return true;
	UItemDefinitionTable* ItemDefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
	UItemDefinition* SourceItemDef = ItemDefTable->GetItemDefinition(SourceSlot.GetItemType());
	UItemDefinition* TargetItemDef = ItemDefTable->GetItemDefinition(TargetSlot.GetItemType());
	if (SourceItemDef->MaxStack == 1 || TargetItemDef->MaxStack == 1)
		return true;
	if (SourceSlot.GetItemType() != TargetSlot.GetItemType())
		return true;
	if (SourceSlot.GetNum() == SourceItemDef->MaxStack
		|| TargetSlot.GetNum() == TargetItemDef->MaxStack)
		return true;
	return false;
}

