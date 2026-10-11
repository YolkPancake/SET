// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "DataAsset/WidgetTable.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Define/SETGameplayTag.h"
#include "WidgetTable.generated.h"

class USETWidgetBase;

UCLASS()
class SET_API UWidgetTable : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UClass* GetWidgetClass(const FGameplayTag& WidgetType);



protected:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "SETGameplayTag.WidgetType"))
	TMap<FGameplayTag, TSoftClassPtr<USETWidgetBase>> SoftWidgetClassMap;

};
