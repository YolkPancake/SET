// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ItemUseAction/ItemUseActionBase.h"
#include "UA_Pistol.generated.h"

/**
 * 
 */
UCLASS()
class SET_API UUA_Pistol : public UItemUseActionBase
{
	GENERATED_BODY()
	
protected:
	virtual void InitUseAction(APlayerController* InLocalPC) override;
	virtual void ResetUseAction() override;
	virtual void HandleInputKey(const FGameplayTag& InKey, const FGameplayTag& InState) override;
	virtual void BreakAllInput() override;
};
