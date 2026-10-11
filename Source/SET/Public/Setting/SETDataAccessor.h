// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Setting/SETDataAccessor.h"

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SETDataAccessor.generated.h"


class UItemDefinitionTable;
class UWidgetTable;
class ULoadingScreen;
class UDDVisualSlot;

UCLASS(Config = Game,defaultconfig)
class SET_API USETDataAccessor : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	static USETDataAccessor* Get();

	UItemDefinitionTable* GetItemDefinitionTable();
	UWidgetTable* GetWidgetTable();
	UClass* GetLoadingScreenClass();
	UClass* GetDDVisualSlotClass();

protected:
	UPROPERTY(EditDefaultsOnly,Config)
	TSoftObjectPtr<UItemDefinitionTable> SoftItemtDefTable;


protected:
	UPROPERTY(EditDefaultsOnly, Config)
	TSoftObjectPtr<UWidgetTable> SoftWidgetTable;


protected:
	UPROPERTY(EditDefaultsOnly,Config)
	TSoftClassPtr<ULoadingScreen> LoadingScreenClass;

protected:
	UPROPERTY(EditDefaultsOnly, Config)
	TSoftClassPtr<UDDVisualSlot> DDVisualSlotClass;
};
