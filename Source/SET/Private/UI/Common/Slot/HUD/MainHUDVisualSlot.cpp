// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Slot/HUD/MainHUDVisualSlot.h"
#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMainHUDVisualSlot::InitVisualSlot(const FInventorySlot& InvSlot, int NewIndex)
{
	SlotIndex = NewIndex;
	RefreshVisualSlot(InvSlot);
}

void UMainHUDVisualSlot::RefreshVisualSlot(const FInventorySlot& InvSlot)
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

int UMainHUDVisualSlot::GetSlotIndex() const
{
	return SlotIndex;
}

void UMainHUDVisualSlot::SetSlotIndex(int NewIndex)
{
	SlotIndex = NewIndex;
}

void UMainHUDVisualSlot::SelectSlot()
{
	BackgroundBorder->SetBrush(SelectedBrush);
}

void UMainHUDVisualSlot::UnseletSlot()
{
	BackgroundBorder->SetBrush(CommonBrush);
}

void UMainHUDVisualSlot::ClearVisualSlot()
{
	IconImg->SetVisibility(ESlateVisibility::Collapsed);
	ItemNumTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}
