// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Slot/HUD/ContainerSlot.h"

#include "Framework/Actor/ContainerActor/ContainerActor.h"
#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"
#include "Subsystem/WidgetManager.h"
#include "UI/Base/DDVisualSlot.h"
#include "UI/Base/SETDragDropOperation.h"
#include "Framework/ActorComponent/ItemTransferComponent.h"
#include "Framework/Controller/GameController.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UContainerSlot::InitContainerSlot(AContainerActor* InContainerActor, const FInventorySlot& InvSlot, int Index)
{
	ContainerActor = InContainerActor;
	SlotIndex = Index;
	RefreshContainerSlot(InvSlot);
}

void UContainerSlot::ResetContainerSlot()
{
	ContainerActor = nullptr;
	SlotIndex = -1;
	ClearSlot();
}

void UContainerSlot::RefreshContainerSlot(const FInventorySlot& InvSlot)
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
		ClearSlot();
	}
}

void UContainerSlot::ClearSlot()
{
	ItemNumTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	IconImg->SetVisibility(ESlateVisibility::Collapsed);
}

void UContainerSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BackgroundBorder->SetBrush(SelectedBrush);
}

void UContainerSlot::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	BackgroundBorder->SetBrush(CommonBrush);
}

FReply UContainerSlot::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	return FReply::Handled();
}

void UContainerSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!ContainerActor.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("ContainerActor在UContainerSlot发生Drag时失效,请检查"));
		return;
	}
	const FInventoryList& InvList = ContainerActor->GetInventory();
	const FInventorySlot& InvSlot = InvList.GetInventorySlotAt(SlotIndex);
	if (!InvSlot.IsValidSlot()) return;
	UWidgetManager* WidgetManager = UWidgetManager::Get(GetOwningPlayer());
	UDDVisualSlot* VisualSlot = WidgetManager->GetDDVisualSlot();
	VisualSlot->RefreshDDVisualSlot(InvSlot);
	USETDragDropOperation* DDOperation = NewObject<USETDragDropOperation>(this);
	DDOperation->DefaultDragVisual = VisualSlot;
	DDOperation->SourceStorable = ContainerActor.Get();
	DDOperation->SourceIndex = SlotIndex;
	OutOperation = DDOperation;
}

bool UContainerSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	if (!ContainerActor.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("ContainerActor在UContainerSlot发生Drop时失效,请检查"));
		return false;
	}
	USETDragDropOperation* DDOperation = Cast<USETDragDropOperation>(InOperation);
	AGameController* GamePC = GetOwningPlayer<AGameController>();
	UItemTransferComponent* TransferComp = GamePC->GetItemTransferComponent();
	TransferComp->Request_TransferItem(DDOperation->SourceStorable, DDOperation->SourceIndex, ContainerActor.Get(), SlotIndex);
	return true;
}
