// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "DataAsset/ItemDefinition.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Define/SETGameplayTag.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "ItemDefinition.generated.h"


class UItemUseActionBase;

UCLASS()
class SET_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "SETGameplayTag.ItemType"))
	FGameplayTag ItemType = SETGameplayTag::ItemType::None;

	UPROPERTY(EditDefaultsOnly)
	FText Name = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer Tag;

	UPROPERTY(EditDefaultsOnly)
	int MaxStack = 50;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> ItemMesh{ nullptr };

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UItemUseActionBase> UseActionClass{ nullptr };

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UAnimInstance> AnimLayer{ nullptr };

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemStateEntry> ItemStateArray;
};
