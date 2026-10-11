// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Interface/Storable.h"

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Define/SETGameplayTag.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "Storable.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChanged, const TArray<int32>&);

UINTERFACE(MinimalAPI)
class UStorable : public UInterface
{
	GENERATED_BODY()
};




class SET_API IStorable
{
	GENERATED_BODY()

public:

	FOnInventoryItemChanged OnInventoryItemChanged;

	//因为监听不跑PostReplicatedChange等,所以所有实现此接口且在权威端都会进行调用
	//由接口实现进行拦截,典型的一种情况是:
	//检测是否是localcontroller.重建cache,通知控件刷新
	//这样即使是其他玩家在监听上的接口实现,虽然会跑,但可以通过逻辑判定进行拦截
	virtual void NotifyInventoryItemChanged(const TArray<int32>& Indexs) = 0;
	virtual FInventoryList& GetInventory() = 0;
	virtual bool CanAcceptItem(const FGameplayTag& ItemType) = 0;
	virtual int32 GetInventorySize() const = 0;
	virtual void PreTransferItem(const int32 Index) {}
	virtual void PostTransferItem(const int32 Index) {}



	
};
