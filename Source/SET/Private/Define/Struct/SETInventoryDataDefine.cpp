// Fill out your copyright notice in the Description page of Project Settings.


#include "Define/Struct/SETInventoryDataDefine.h"

#include "Interface/Storable.h"


// Begin FInventoryItemData

bool FInventoryItemData::IsValid() const
{
	return ItemType != SETGameplayTag::ItemType::None;
}

int32 FInventoryItemData::GetNum() const
{
	return CurrentNum;
}

float FInventoryItemData::GetStateValue(const FGameplayTag& StateTag) const
{
	for (const FItemStateEntry& Entry : StateEntry)
	{
		if (Entry.StateTag == StateTag)
		{
			return Entry.Data;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("实际数据%s中没有类别为%s的状态标签,请查看配置表设置,get默认返回0"),*ItemType.ToString(),*StateTag.ToString());
	return 0;
}

void FInventoryItemData::SetNum(const int32 NewNum)
{
	CurrentNum = NewNum;
}

void FInventoryItemData::SetStateValue(const FGameplayTag& StateTag, const int32 NewValue)
{
	for (FItemStateEntry& Entry : StateEntry)
	{
		if (Entry.StateTag == StateTag)
		{
			Entry.Data = NewValue;
			return;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("实际数据%s中没有类别为%s的状态标签,请查看配置表设置,无法进行Set"), *ItemType.ToString(), *StateTag.ToString());
}

void FInventoryItemData::Clear()
{
	ItemType = SETGameplayTag::ItemType::None;
	StateEntry.Empty();
	SetNum(0);
}

// End FInventoryItemData


// Begin FInventorySlot

bool FInventorySlot::EqualTo(const FInventorySlot& Other)
{
	if (GetItemType() != Other.GetItemType()) return false;
	for (FItemStateEntry State : ItemData.StateEntry)
		if (State.Data != Other.GetStateValue(State.StateTag))
			return false;
	return true;
}

const FGameplayTag& FInventorySlot::GetItemType() const
{
	return ItemData.ItemType;
}

bool FInventorySlot::IsValidSlot() const
{
	return ItemData.IsValid();
}

int32 FInventorySlot::GetNum() const
{
	return ItemData.GetNum();
}

float FInventorySlot::GetStateValue(const FGameplayTag& StateTag) const
{
	return ItemData.GetStateValue(StateTag);
}

const FInventoryItemData& FInventorySlot::GetItemData() const
{
	return ItemData;
}

void FInventorySlot::ClearSlot()
{
	ItemData.Clear();
}

// End FInventorySlot


// Begin FInventoryList

bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FInventorySlot, FInventoryList>(
		SlotList, DeltaParms, *this);
}

void FInventoryList::InitList(const int32 Num)
{
	SlotList.SetNum(Num);
	for (FInventorySlot& InvSlot : SlotList)
	{
		InvSlot.ClearSlot();
		MarkItemDirty(InvSlot);
	}

	//补偿监听
	if (OwnerStorable.Get())
	{
		TArray<int32> Indexs;
		Indexs.Reserve(Num);
		for (int32 i = 0; i < Num; i++)
		{
			Indexs.Add(i);
		}
		OwnerStorable.Get()->NotifyInventoryItemChanged(Indexs);
	}	
}

// 监听永远不跑,所以在四个修改API中对监听进行补偿
// 核心用途就是通知客户端库存发生了变化,由实现方刷新控件
void FInventoryList::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	if (OwnerStorable.Get())
	{
		TArray<int32> Indexs{ ChangedIndices.GetData(), ChangedIndices.Num() };
		OwnerStorable.Get()->NotifyInventoryItemChanged(Indexs);
	}
}

// 监听永远不跑,在InitList中补偿监听
// 核心用途就是通知客户端库存发生了变化,由实现方刷新控件
void FInventoryList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	if (OwnerStorable.Get())
	{
		TArray<int32> Indexs{ AddedIndices.GetData(), AddedIndices.Num() };
		OwnerStorable.Get()->NotifyInventoryItemChanged(Indexs);
	}
}

int32 FInventoryList::GetSize() const
{
	return SlotList.Num();
}

const FGameplayTag& FInventoryList::GetItemTypeAt(const int32 Index) const
{
	return GetInventorySlotAt(Index).GetItemType();
}

const FInventorySlot& FInventoryList::GetInventorySlotAt(const int Index) const
{
	check(SlotList.IsValidIndex(Index));
	return SlotList[Index];
}

int32 FInventoryList::GetItemNumAt(const int32 Index) const
{
	check(SlotList.IsValidIndex(Index));
	return SlotList[Index].GetNum();
}

float FInventoryList::GetStateValueAt(const int32 Index, const FGameplayTag& StateTag) const
{
	check(SlotList.IsValidIndex(Index));
	return SlotList[Index].GetStateValue(StateTag);
}

bool FInventoryList::IsValidSlotAt(const int32 Index) const
{
	check(SlotList.IsValidIndex(Index));
	return SlotList[Index].IsValidSlot();
}

void FInventoryList::SetSlotAt(const FInventorySlot& NewSlot, int32 Index)
{
	SetSlotAt(NewSlot.ItemData, Index);
}

void FInventoryList::SetSlotAt(const FInventoryItemData& NewData, int32 Index)
{
	check(SlotList.IsValidIndex(Index));

	FInventorySlot& InvSlot = SlotList[Index];
	InvSlot.ItemData = NewData;

	MarkItemDirty(InvSlot);

	// 所有在权威上的都会调用,用来补偿监听不会触发PostReplicatedChange函数
	// 但因为在权威上的所有都会跑,需要实际实现了IStorable自行设置拦截逻辑
	if (OwnerStorable.Get())
	{
		TArray<int32> ChangdeIndex{ Index };
		OwnerStorable.Get()->NotifyInventoryItemChanged(ChangdeIndex);
	}
}

void FInventoryList::ClearSlotAt(const int32 Index)
{
	check(SlotList.IsValidIndex(Index));

	SlotList[Index].ClearSlot();
	MarkItemDirty(SlotList[Index]);

	if (OwnerStorable.Get())
	{
		TArray<int32> ChangdeIndex{ Index };
		OwnerStorable.Get()->NotifyInventoryItemChanged(ChangdeIndex);
	}
}

void FInventoryList::SetItemNumAt(const int32 Index, const int32 NewNum)
{
	check(SlotList.IsValidIndex(Index));

	FInventorySlot& InvSlot = SlotList[Index];
	InvSlot.ItemData.SetNum(NewNum);

	MarkItemDirty(InvSlot);

	if (OwnerStorable.Get())
	{
		TArray<int32> ChangdeIndex{ Index };
		OwnerStorable.Get()->NotifyInventoryItemChanged(ChangdeIndex);
	}
}

void FInventoryList::SetStateValueAt(const int32 Index, const FGameplayTag& StateTag, const int32 Value)
{
	check(SlotList.IsValidIndex(Index));

	FInventorySlot& InvSlot = SlotList[Index];
	InvSlot.ItemData.SetStateValue(StateTag, Value);

	MarkItemDirty(InvSlot);

	if (OwnerStorable.Get())
	{
		TArray<int32> ChangdeIndex{ Index };
		OwnerStorable.Get()->NotifyInventoryItemChanged(ChangdeIndex);
	}
}

// End FInventoryList
