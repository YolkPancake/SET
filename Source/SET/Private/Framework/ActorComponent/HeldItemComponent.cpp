// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ActorComponent/HeldItemComponent.h"

#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "Framework/Controller/GameController.h"
#include "Framework/Character/GameCharacter.h"
#include "Components/StaticMeshComponent.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"

UHeldItemComponent::UHeldItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;



}

void UHeldItemComponent::BeginPlay()
{
	Super::BeginPlay();

	AGameCharacter* GameCA = GetOwner<AGameCharacter>();
	ItemMeshComp = GameCA->GetHeldItemMeshComponent();
	GameCA->OnPlayerReady.AddUObject(this, &UHeldItemComponent::OnPlayerReady_CallBack);
}

void UHeldItemComponent::OnHeldItemChanged_CallBack()
{
	const FInventoryList& QBList = QBComp->GetInventory();
	const FInventorySlot& InvSlot = QBList.GetInventorySlotAt(QBComp->GetCurrentQuickBarIndex());
	if (InvSlot.IsValidSlot())
	{
		ItemMeshComp->SetVisibility(true);
		UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
		UItemDefinition* ItemDef = DefTable->GetItemDefinition(InvSlot.GetItemType());
		ItemMeshComp->SetStaticMesh(ItemDef->ItemMesh.LoadSynchronous());
		// TODO : 连接动画层,让手出现不同的动画
		/*
		AGameCharacter* GameCA = GetOwner<AGameCharacter>();
		GameCA->GetArmMesh()->LinkAnimClassLayers(ItemDef->AnimLayer.LoadSynchronous());
		*/
	}
	else ItemMeshComp->SetVisibility(false);
}

void UHeldItemComponent::OnPlayerReady_CallBack()
{
	AGameCharacter* GameCA = GetOwner<AGameCharacter>();
	AGameController* GamePC = GameCA->GetController<AGameController>();
	QBComp = GamePC->GetQuickBarComponent();
	QBComp->OnHeldItemChanged.AddUObject(this, &UHeldItemComponent::OnHeldItemChanged_CallBack);
}
