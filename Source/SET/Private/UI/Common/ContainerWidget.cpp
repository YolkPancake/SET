// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/ContainerWidget.h"

#include "UI/Common/Slot/HUD/ContainerSlot.h"
#include "UI/ActivationContext/InventoryHUDContext.h"
#include "Framework/Actor/ContainerActor/ContainerActor.h"

#include "Components/UniformGridPanel.h"
void UContainerWidget::InitContainerWidget(UInventoryHUDContext* InCtx)
{
	AContainerActor* Container = InCtx->ThisContainerActor.Get();
	if (!Container) return;
	ContainerActor = Container;
	const FInventoryList& InvList = Container->GetInventory();
	for (int i = 0; i < Container->GetInventorySize(); i++)
	{
		UContainerSlot* ContainerSlot = GetContainerSlot();
		ContainerSlot->InitContainerSlot(Container, InvList.GetInventorySlotAt(i),i);
		ContainerSlotArray.Add(ContainerSlot);
		GridPanel->AddChildToUniformGrid(ContainerSlot, i / 10, i % 10);
	}
	ContainerItemChanged_Handle = Container->OnInventoryItemChanged.AddUObject(this, &UContainerWidget::OnContainerItemChanged_CallBack);
}

void UContainerWidget::ResetContainerWidget()
{
	GridPanel->ClearChildren();
	ContainerSlotArray.Empty();
	while (UsedSlotArray.Num()>0)
	{
		UContainerSlot* UsedSlot = UsedSlotArray.Pop();
		UsedSlot->ResetContainerSlot();
		AvailableSlotArray.Add(UsedSlot);
	}

	if (ContainerActor.Get())
	{
		ContainerActor->OnInventoryItemChanged.Remove(ContainerItemChanged_Handle);
		ContainerActor = nullptr;
	}
}

UContainerSlot* UContainerWidget::GetContainerSlot()
{
	if (AvailableSlotArray.Num() > 0)
	{
		UContainerSlot* ExistSlot = AvailableSlotArray.Pop();
		UsedSlotArray.Add(ExistSlot);
		return ExistSlot;
	}
	UContainerSlot* NewSlot = CreateWidget<UContainerSlot>(GetOwningPlayer(), ContainerSlotClass);
	UsedSlotArray.Add(NewSlot);
	return NewSlot;
}

void UContainerWidget::OnContainerItemChanged_CallBack(const TArray<int>& Indexs)
{
	if (!ContainerActor.Get()) return;
	const FInventoryList& InvList = ContainerActor.Get()->GetInventory();
	for (const int& Index : Indexs)
		ContainerSlotArray[Index]->RefreshContainerSlot(InvList.GetInventorySlotAt(Index));
}
