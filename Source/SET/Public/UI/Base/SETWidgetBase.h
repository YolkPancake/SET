// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Base/SETWidgetBase.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/SETGameplayTag.h"
#include "Define/Struct/WidgetInputConfig.h"
#include "SETWidgetBase.generated.h"


class UWidgetActivationContext;
class UWidgetManager;



UCLASS(Abstract, Blueprintable)
class SET_API USETWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void OnActivated(UWidgetActivationContext* Ctx);
	virtual void OnDeactivated();
	virtual void HandleInputKey(const FGameplayTag& InKey) {}
	void RegisterWidgetManager(UWidgetManager* Manager);
	const FWidgetInputConfig& GetRequiredInputConfig() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FWidgetInputConfig InputConfig;

protected:
	// RuntimeCache
	UPROPERTY(Transient)
	TWeakObjectPtr<UWidgetManager> WidgetManager;
};
