// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screen/HUD/HUD_MainHUD.h"

#include "Framework/Controller/GameController.h"
#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "UI/Common/Slot/HUD/MainHUDVisualSlot.h"
#include "Subsystem/WidgetManager.h"

#include "Components/UniformGridPanel.h"


void UHUD_MainHUD::HandleInputKey(const FGameplayTag& InKey)
{
	if (InKey == SETGameplayTag::WidgetInputKey::Esc)
	{

		return;
	}

	if (InKey == SETGameplayTag::WidgetInputKey::Tab)
	{
		WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::HUD::InventoryHUD);
		return;
	}

}

void UHUD_MainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerQuickBarComponent* QBComp = GamePC->GetQuickBarComponent();
	const FInventoryList& InvList = QBComp->GetInventory();
	CurrentQBIndex = QBComp->GetCurrentQuickBarIndex();
	for (int i = 0; i < QBComp->GetInventorySize(); i++)
	{
		UMainHUDVisualSlot* NewSlot = CreateWidget<UMainHUDVisualSlot>(GetOwningPlayer(), VisualSlotClass);
		VisualSlotArray.Add(NewSlot);
		NewSlot->InitVisualSlot(InvList.GetInventorySlotAt(i),i);
		GridPanel->AddChildToUniformGrid(NewSlot, 0, i);
	}
	VisualSlotArray[CurrentQBIndex]->SelectSlot();
	QBComp->OnCurrentQuikcBarIndexChanged.AddUObject(this, &UHUD_MainHUD::OnCurrentQuickBarIndexChanged_CallBack);
	QBComp->OnInventoryItemChanged.AddUObject(this, &UHUD_MainHUD::OnQuickBarItemChanged_CallBack);
}

void UHUD_MainHUD::OnCurrentQuickBarIndexChanged_CallBack(const int& NewIndex)
{
	VisualSlotArray[CurrentQBIndex]->UnseletSlot();
	CurrentQBIndex = NewIndex;
	VisualSlotArray[CurrentQBIndex]->SelectSlot();
}

void UHUD_MainHUD::OnQuickBarItemChanged_CallBack(const TArray<int>&Indexs)
{
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerQuickBarComponent* QBComp = GamePC->GetQuickBarComponent();
	const FInventoryList& InvList = QBComp->GetInventory();
	for (const int& Index : Indexs)
		VisualSlotArray[Index]->RefreshVisualSlot(InvList.GetInventorySlotAt(Index));
}
