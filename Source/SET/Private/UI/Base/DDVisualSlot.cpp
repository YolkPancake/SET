// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/DDVisualSlot.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDDVisualSlot::RefreshDDVisualSlot(const FInventorySlot& InvSlot)
{
	UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
	UItemDefinition* ItemDef = DefTable->GetItemDefinition(InvSlot.GetItemType());
	IconImg->SetBrushFromSoftTexture(ItemDef->Icon);
	ItemNumTextBlock->SetText(FText::AsNumber(InvSlot.GetNum()));
}
