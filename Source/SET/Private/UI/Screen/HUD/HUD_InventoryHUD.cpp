// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screen/HUD/HUD_InventoryHUD.h"

#include "UI/ActivationContext/InventoryHUDContext.h"
#include "Framework/Controller/GameController.h"
#include "Framework/ActorComponent/PlayerBagComponent.h"
#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "Subsystem/WidgetManager.h"
#include "UI/Common/Slot/HUD/QuickBarSlot.h"
#include "UI/Common/Slot/HUD/BagSlot.h"
#include "UI/Common/ContainerWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"

void UHUD_InventoryHUD::OnActivated(UWidgetActivationContext* Ctx)
{
	Super::OnActivated(Ctx);
	if (UInventoryHUDContext* InventoryCtx = Cast<UInventoryHUDContext>(Ctx))
	{
		WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		DoInitSubWidget(InventoryCtx);
	}
	else
	{
		WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHUD_InventoryHUD::OnDeactivated()
{
	Super::OnDeactivated();

	SubContainerWidget->ResetContainerWidget();
}

void UHUD_InventoryHUD::DoInitSubWidget(UInventoryHUDContext* Ctx)
{
	SubContainerWidget->InitContainerWidget(Ctx);
}

void UHUD_InventoryHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerQuickBarComponent* QBComp = GamePC->GetQuickBarComponent();
	UPlayerBagComponent* BagComp = GamePC->GetBagComponent();
	const FInventoryList& QBList = QBComp->GetInventory();
	const FInventoryList& InvList = BagComp->GetInventory();
	for (int i = 0 ; i < QBComp->GetInventorySize(); i++)
	{
		UQuickBarSlot* NewSlot = CreateWidget<UQuickBarSlot>(GamePC, QuickBarSlotClass);
		NewSlot->InitQuickBarSlot(QBList.GetInventorySlotAt(i), i);
		QBSlotArray.Add(NewSlot);
		QuickBarGridPanel->AddChildToUniformGrid(NewSlot, 0,i);
	}
	for (int i = 0; i < BagComp->GetInventorySize(); i++)
	{
		UBagSlot* NewSlot = CreateWidget<UBagSlot>(GamePC, BagSlotClass);
		NewSlot->InitInventorySlot(InvList.GetInventorySlotAt(i), i);
		BagSlotArray.Add(NewSlot);
		InventoryGridPanel->AddChildToUniformGrid(NewSlot, i / 10, i % 10);
	}
	QBComp->OnInventoryItemChanged.AddUObject(this, &UHUD_InventoryHUD::OnQuikBarItemChanged_CallBack);
	BagComp->OnInventoryItemChanged.AddUObject(this, &UHUD_InventoryHUD::OnBagItemChanged_CallBack);
}

FReply UHUD_InventoryHUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Escape || Key == EKeys::Tab)
		WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::HUD::MainHUD);
	return FReply::Handled();
}

void UHUD_InventoryHUD::OnQuikBarItemChanged_CallBack(const TArray<int>& Indexs)
{
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerQuickBarComponent* QBComp = GamePC->GetQuickBarComponent();
	const FInventoryList& QBList = QBComp->GetInventory();
	for (const int& Index : Indexs)
		QBSlotArray[Index]->RefreshQuickBarSlot(QBList.GetInventorySlotAt(Index));
}

void UHUD_InventoryHUD::OnBagItemChanged_CallBack(const TArray<int>&Indexs)
{
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerBagComponent* BagComp = GamePC->GetBagComponent();
	const FInventoryList& InvList = BagComp->GetInventory();
	for (const int& Index : Indexs)
		BagSlotArray[Index]->RefreshBagSlot(InvList.GetInventorySlotAt(Index));
}

