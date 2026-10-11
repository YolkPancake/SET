// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Slot/HUD/BagSlot.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"
#include "UI/Base/SETDragDropOperation.h"
#include "Framework/Controller/GameController.h"
#include "Framework/ActorComponent/PlayerBagComponent.h"
#include "Subsystem/WidgetManager.h"
#include "UI/Base/DDVisualSlot.h"
#include "Framework/ActorComponent/ItemTransferComponent.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UBagSlot::InitInventorySlot(const FInventorySlot& InvSlot, int NewIndex)
{
	SlotIndex = NewIndex;
	RefreshBagSlot(InvSlot);
}

void UBagSlot::RefreshBagSlot(const FInventorySlot & InvSlot)
{
	if (InvSlot.IsValidSlot())
	{
		UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
		UItemDefinition* ItemDef = DefTable->GetItemDefinition(InvSlot.GetItemType());
		IconImg->SetBrushFromSoftTexture(ItemDef->Icon);
		ItemNumTextBlock->SetText(FText::AsNumber(InvSlot.GetNum()));
		IconImg->SetVisibility(ESlateVisibility::Visible);
		ItemNumTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ClearVisualSlot();
	}
}

int UBagSlot::GetSlotIndex() const
{
	return SlotIndex;
}

void UBagSlot::SetSlotIndex(int NewIndex)
{
	SlotIndex = NewIndex;
}

void UBagSlot::ClearVisualSlot()
{
	IconImg->SetVisibility(ESlateVisibility::Collapsed);
	ItemNumTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}

void UBagSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BackgroundBorder->SetBrush(SelectedBrush);
}

void UBagSlot::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	BackgroundBorder->SetBrush(CommonBrush);
}

FReply UBagSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	return FReply::Handled();
}

void UBagSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UPlayerBagComponent* BagComp = GamePC->GetBagComponent();
	const FInventoryList& BagList = BagComp->GetInventory();
	const FInventorySlot& BagSlot = BagList.GetInventorySlotAt(SlotIndex);
	if (BagSlot.GetItemType() == SETGameplayTag::ItemType::None)
		return;
	UWidgetManager* WidgetManager = UWidgetManager::Get(GamePC);
	UDDVisualSlot* VisualSlot = WidgetManager->GetDDVisualSlot();
	VisualSlot->RefreshDDVisualSlot(BagSlot);
	USETDragDropOperation* DDOperation = NewObject<USETDragDropOperation>(this);
	DDOperation->DefaultDragVisual = VisualSlot;
	DDOperation->SourceStorable = BagComp;
	DDOperation->SourceIndex = SlotIndex;
	OutOperation = DDOperation;
}

bool UBagSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	USETDragDropOperation* DDOperation = Cast<USETDragDropOperation>(InOperation);
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UItemTransferComponent* TransferComp = GamePC->GetItemTransferComponent();
	UPlayerBagComponent* BagComp = GamePC->GetBagComponent();
	TransferComp->Request_TransferItem(DDOperation->SourceStorable, DDOperation->SourceIndex, BagComp, SlotIndex);
	return true;
}
