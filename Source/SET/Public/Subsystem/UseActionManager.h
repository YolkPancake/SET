// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Subsystem/UseActionManager.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Define/SETGameplayTag.h"
#include "UseActionManager.generated.h"

class UItemUseActionBase;

//  本地的UseActionManager,负责创建UseAction,


UCLASS()
class SET_API UUseActionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UUseActionManager* Get(UObject* InWorldCtxObj);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UItemUseActionBase* GetItemUseAction(APlayerController* InLcoalPC,const FGameplayTag& InItemType);

};
