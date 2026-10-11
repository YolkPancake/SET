// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Slot/HUD/QuickBarSlot.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"
#include "UI/Base/SETDragDropOperation.h"
#include "Framework/Controller/GameController.h"
#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "Subsystem/WidgetManager.h"
#include "UI/Base/DDVisualSlot.h"
#include "Framework/ActorComponent/ItemTransferComponent.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UQuickBarSlot::InitQuickBarSlot(const FInventorySlot& InvSlot, int NewIndex)
{
	SlotIndex = NewIndex;
	RefreshQuickBarSlot(InvSlot);
}

void UQuickBarSlot::RefreshQuickBarSlot(const FInventorySlot & InvSlot)
{
	if (InvSlot.GetItemType() == SETGameplayTag::ItemType::None)
	{
		ClearVisualSlot();
	}
	else
	{
		UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
		UItemDefinition* ItemDef = DefTable->GetItemDefinition(InvSlot.GetItemType());
		IconImg->SetBrushFromSoftTexture(ItemDef->Icon);
		ItemNumTextBlock->SetText(FText::AsNumber(InvSlot.GetNum()));
		IconImg->SetVisibility(ESlateVisibility::Visible);
		ItemNumTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

int UQuickBarSlot::GetSlotIndex() const
{
	return SlotIndex;
}

void UQuickBarSlot::SetSlotIndex(int NewIndex)
{
	SlotIndex = NewIndex;
}

void UQuickBarSlot::ClearVisualSlot()
{
	IconImg->SetVisibility(ESlateVisibility::Collapsed);
	ItemNumTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}

void UQuickBarSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BackgroundBorder->SetBrush(SelectedBrush);
}

void UQuickBarSlot::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	BackgroundBorder->SetBrush(CommonBrush);
}

FReply UQuickBarSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	return FReply::Handled();
}

void UQuickBarSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerQuickBarComponent* QBComp = GamePC->GetQuickBarComponent();
	const FInventoryList& QBList = QBComp->GetInventory();
	const FInventorySlot& QBSlot = QBList.GetInventorySlotAt(SlotIndex);
	if (QBSlot.GetItemType() == SETGameplayTag::ItemType::None)
		return;
	UWidgetManager* WidgetManager = UWidgetManager::Get(GamePC);
	UDDVisualSlot* VisualSlot = WidgetManager->GetDDVisualSlot();
	VisualSlot->RefreshDDVisualSlot(QBSlot);
	USETDragDropOperation* DDOperation = NewObject<USETDragDropOperation>(this);
	DDOperation->DefaultDragVisual = VisualSlot;
	DDOperation->SourceStorable = QBComp;
	DDOperation->SourceIndex = SlotIndex;
	OutOperation = DDOperation;
}

bool UQuickBarSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	USETDragDropOperation* DDOperation = Cast<USETDragDropOperation>(InOperation);
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UItemTransferComponent* TransferComp = GamePC->GetItemTransferComponent();
	UPlayerQuickBarComponent* QBComp = GamePC->GetQuickBarComponent();
	TransferComp->Request_TransferItem(DDOperation->SourceStorable, DDOperation->SourceIndex, QBComp, SlotIndex);
	return true;
}
