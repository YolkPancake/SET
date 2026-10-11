// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/SETWidgetBase.h"
#include "HUD_MainHUD.generated.h"


class UMainHUDVisualSlot;
class UUniformGridPanel;



UCLASS(Abstract, Blueprintable)
class SET_API UHUD_MainHUD : public USETWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void HandleInputKey(const FGameplayTag& InKey) override;
	virtual void NativeOnInitialized() override;

	void OnCurrentQuickBarIndexChanged_CallBack(const int& NewIndex);
	void OnQuickBarItemChanged_CallBack(const TArray<int>& Indexs);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UMainHUDVisualSlot> VisualSlotClass;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UMainHUDVisualSlot>> VisualSlotArray;

	UPROPERTY(Transient)
	int CurrentQBIndex = 0;
};
