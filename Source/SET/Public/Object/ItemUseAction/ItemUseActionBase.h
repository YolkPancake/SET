// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Object/ItemUseAction/ItemUseActionBase.h"

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "ItemUseActionBase.generated.h"


// 本地逻辑执行器,负责调度,

UCLASS()
class SET_API UItemUseActionBase : public UObject
{
	GENERATED_BODY()
	
public:
	// 手持时触发
	virtual void InitUseAction(APlayerController* InLocalPC);
	// 不再手持时触发
	virtual void ResetUseAction();

	virtual void HandleInputKey(const FGameplayTag& InKey, const FGameplayTag& InState) {}
	virtual void BreakAllInput() {}




protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> LocalPC;
};
