// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Actor/ContainerActor/ContainerActor.h"

#include "Framework/ActorComponent/RPCComponent.h"
#include "Framework/Controller/GameController.h"
#include "Subsystem/WidgetManager.h"
#include "UI/ActivationContext/InventoryHUDContext.h"
#include "Net/UnrealNetwork.h"

AContainerActor::AContainerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;


}

void AContainerActor::Local_OnInteract(APlayerController* InInstigator)
{
	UWidgetManager* WidgetManager = UWidgetManager::Get(InInstigator);
	UInventoryHUDContext* Ctx = NewObject<UInventoryHUDContext>(this);
	Ctx->ThisContainerActor = this;
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::HUD::InventoryHUD, Ctx);
}

void AContainerActor::Authority_OnInteract(APlayerController * InInstigator)
{

}

bool AContainerActor::ShouldInteractOnLocal() const
{
	return true;
}

bool AContainerActor::ShouldInteractOnAuthority() const
{
	return false;
}

void AContainerActor::NotifyInventoryItemChanged(const TArray<int32>& Indexs)
{
	OnInventoryItemChanged.Broadcast(Indexs);
}

FInventoryList& AContainerActor::GetInventory()
{
	return InvList;
}

bool AContainerActor::CanAcceptItem(const FGameplayTag& ItemType)
{
	return true;
}

int32 AContainerActor::GetInventorySize() const
{
	return InvList.GetSize();
}

void AContainerActor::Local_OpenContainerHUD(APlayerController* InInstigator)
{
	UWidgetManager* WidgetManager = UWidgetManager::Get(InInstigator);
	UInventoryHUDContext* Ctx = NewObject<UInventoryHUDContext>(this);
	Ctx->ThisContainerActor = this;
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::HUD::InventoryHUD, Ctx);
}

void AContainerActor::Local_RemoveContainerHUD(APlayerController* InInstigator)
{
	UWidgetManager* WidgetManager = UWidgetManager::Get(InInstigator);
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::HUD::MainHUD);
}

void AContainerActor::Authority_RemoveViewer(APlayerController* InViewer)
{
	Viewer.RemoveSingle(InViewer);
}

void AContainerActor::BeginPlay()
{
	Super::BeginPlay();
	
	InvList.OwnerStorable = this;
	if (HasAuthority())
	{
		InvList.InitList(50);
	}

}

void AContainerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AContainerActor, InvList, ELifetimeCondition::COND_None);
}

