// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "DataAsset/ItemDefinitionTable.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Define/SETGameplayTag.h"
#include "ItemDefinitionTable.generated.h"


class UItemDefinition;

UCLASS()
class SET_API UItemDefinitionTable : public UDataAsset
{
	GENERATED_BODY()
	

public:
	UItemDefinition* GetItemDefinition(const FGameplayTag& ItemType);


	UPROPERTY(EditAnywhere, meta = (Categories = "SETGameplayTag.ItemType"))
	TMap<FGameplayTag, TSoftObjectPtr<UItemDefinition>> ItemDefMap;
};
