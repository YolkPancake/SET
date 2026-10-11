// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Define/Struct/SETInventoryDataDefine.h"

#include "Net/Serialization/FastArraySerializer.h"
#include "Define/SETGameplayTag.h"
#include "SETInventoryDataDefine.generated.h"


class IStorable;

USTRUCT()
struct FItemStateEntry
{
	GENERATED_BODY()

public:
	FItemStateEntry() = default;

	UPROPERTY(EditAnywhere,meta = (Categories = "SETGameplayTag.ItemState"))
	FGameplayTag StateTag = SETGameplayTag::ItemState::None;

	UPROPERTY(EditAnywhere)
	float Data = 0;
};


/**
 * 物品数据:值语义,只描述"是什么 / 有多少 / 什么状态"
 * 可以自由构造与修改,是各个容器(掉落物、背包、快捷栏)之间流转的载体
 */
USTRUCT()
struct FInventoryItemData
{
	GENERATED_BODY()

public:
	FInventoryItemData() = default;

	UPROPERTY(EditAnywhere)
	FGameplayTag ItemType = SETGameplayTag::ItemType::None;

	UPROPERTY(EditAnywhere)
	int CurrentNum = 0;

	UPROPERTY(EditAnywhere)
	TArray<FItemStateEntry> StateEntry;

public:
	bool IsValid() const;
	int32 GetNum() const;
	float GetStateValue(const FGameplayTag& StateTag) const;

public:
	// 值对象:允许自由写入,用于构造/填充数据
	void SetNum(const int32 NewNum);
	void SetStateValue(const FGameplayTag& StateTag, const int32 NewValue);
	void Clear();



};


/**
 * 库存槽位:对外只暴露访问器,所有写入都必须通过 FInventoryList 的按索引 API
 * (这样才能统一做 MarkItemDirty 与缓存重建)
 */
USTRUCT()
struct FInventorySlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

	friend struct FInventoryList;

public:
	FInventorySlot() = default;

protected:
	UPROPERTY()
	FInventoryItemData ItemData;


public:
	bool EqualTo(const FInventorySlot& Other);
	const FGameplayTag& GetItemType() const;
	bool IsValidSlot() const;
	int32 GetNum() const;
	float GetStateValue(const FGameplayTag& StateTag) const;
	const FInventoryItemData& GetItemData() const;

protected:
	void ClearSlot();
};


USTRUCT()
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TArray<FInventorySlot> SlotList;

public:

	TWeakInterfacePtr<IStorable> OwnerStorable;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void InitList(const int32 Num);

	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);

	// Begin Getter
	int32 GetSize() const;
	const FGameplayTag& GetItemTypeAt(const int32 Index) const;
	const FInventorySlot& GetInventorySlotAt(const int32 Index) const;
	int32 GetItemNumAt(const int32 Index) const;
	float GetStateValueAt(const int32 Index, const FGameplayTag& StateTag) const;
	bool IsValidSlotAt(const int32 Index) const;
	// End Getter

	// Begin Setter
	void SetSlotAt(const FInventorySlot& NewSlot, int32 Index);
	void SetSlotAt(const FInventoryItemData& NewData, int32 Index);
	void ClearSlotAt(const int32 Index);
	void SetItemNumAt(const int32 Index, const int32 NewNum);
	void SetStateValueAt(const int32 Index, const FGameplayTag& StateTag, const int32 Value);
	// End Setter

};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
