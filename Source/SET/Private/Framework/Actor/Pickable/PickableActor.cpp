// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Actor/Pickable/PickableActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"
#include "Framework/Character/GameCharacter.h"
#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"
#include "Net/UnrealNetwork.h"
#include "Framework/Controller/GameController.h"
#include "Framework/ActorComponent/PlayerBagComponent.h"

APickableActor::APickableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SphereArea = CreateDefaultSubobject<USphereComponent>(TEXT("SphereArea"));
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	Billboard->SetVisibility(false);



	SetRootComponent(ItemMesh);
	SphereArea->SetupAttachment(GetRootComponent());
	Billboard->SetupAttachment(GetRootComponent());

	SphereArea->OnComponentBeginOverlap.AddDynamic(this, &APickableActor::OnSphereAreaBeginOverlap_CallBack);
	SphereArea->OnComponentEndOverlap.AddDynamic(this, &APickableActor::OnSphereAreaEndOverlap_CallBack);
}

void APickableActor::Authority_InitPickableActor(const FInventoryItemData& Data)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("APickableActor被要求初始化,但当前并非权威环境,拒绝初始化"));
		return;
	}

	ItemData = Data;
	OnRep_ItemData();
}

void APickableActor::Authority_OnInteract(APlayerController* InInstigator)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("APickableActor被调用权威互动,但当前环境并非权威!"));
		return;
	}
	Authority_OnInteracte(InInstigator);
}

bool APickableActor::ShouldInteractOnLocal() const
{
	return false;
}

bool APickableActor::ShouldInteractOnAuthority() const
{
	return true;
}

void APickableActor::BeginPlay()
{
	Super::BeginPlay();

	// 测试用初始化,后续删除,通过外部调用Authority_InitPickableActor进行初始化
	if (HasAuthority())
	{
		Authority_InitPickableActor(TestData);
	}

}

void APickableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APickableActor, ItemData, ELifetimeCondition::COND_None);
}

void APickableActor::OnSphereAreaBeginOverlap_CallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter* GameCA = Cast<AGameCharacter>(OtherActor);
	if (!GameCA || !GameCA->IsLocallyControlled()) return;
	Billboard->SetVisibility(true);
}

void APickableActor::OnSphereAreaEndOverlap_CallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGameCharacter* GameCA = Cast<AGameCharacter>(OtherActor);
	if (!GameCA || !GameCA->IsLocallyControlled()) return;
	Billboard->SetVisibility(false);
}

void APickableActor::Authority_OnInteracte(APlayerController* InInstigator)
{
	AGameController* GamePC = Cast<AGameController>(InInstigator);
	UPlayerBagComponent* BagComp = GamePC->GetBagComponent();
	const int32 Remaining = BagComp->Request_AddItem(ItemData);
	if (Remaining <= 0)
	{
		Destroy();
	}
	else
	{
		ItemData.CurrentNum = Remaining;
		OnRep_ItemData();
	}
}

void APickableActor::OnRep_ItemData()
{
	UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
	UItemDefinition* ItemDef = DefTable->GetItemDefinition(ItemData.ItemType);
	ItemMesh->SetStaticMesh(ItemDef->ItemMesh.LoadSynchronous());
}
