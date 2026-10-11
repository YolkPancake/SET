// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/SETWidgetBase.h"
#include "HUD_InventoryHUD.generated.h"

class UInventoryHUDContext;

class UUniformGridPanel;
class UQuickBarSlot;
class UBagSlot;
class UWidgetSwitcher;
class UContainerWidget;

UCLASS(Abstract, Blueprintable)
class SET_API UHUD_InventoryHUD : public USETWidgetBase
{
	GENERATED_BODY()
	

protected:
	virtual void OnActivated(UWidgetActivationContext* Ctx) override;
	virtual void OnDeactivated() override;

	void DoInitSubWidget(UInventoryHUDContext* Ctx);


protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void OnQuikBarItemChanged_CallBack(const TArray<int>& Indexs);
	void OnBagItemChanged_CallBack(const TArray<int>& Indexs);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> QuickBarGridPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	// SubWidget
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UContainerWidget> SubContainerWidget;



protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuickBarSlot> QuickBarSlotClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBagSlot> BagSlotClass;


	UPROPERTY(Transient)
	TArray<TObjectPtr<UQuickBarSlot>> QBSlotArray;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UBagSlot>> BagSlotArray;
};
