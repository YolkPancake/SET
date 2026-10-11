

#include "Framework/ActorComponent/InteractComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

#include "Interface/Interactable.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UInteractComponent::DoOnceInteract()
{
	ACharacter* CA = GetOwner<ACharacter>();
	APlayerController* PC = CA->GetController<APlayerController>();
	FVector Location{};
	FRotator Rotation{};
	PC->GetPlayerViewPoint(Location, Rotation);
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bTraceComplex = false;
	Params.bDebugQuery = true;
	GetWorld()->LineTraceSingleByChannel(Result, Location, Location + (Rotation.Vector() * TraceLength), ECollisionChannel::ECC_GameTraceChannel10, Params);
	IInteractable* Interactable = Cast<IInteractable>(Result.GetActor());
	if (!Interactable) return;
	DrawDebugLine(GetWorld(), Location, Location + (Rotation.Vector() * TraceLength), FColor::Cyan, true);
	if (Interactable->ShouldInteractOnLocal())
		Interactable->Local_OnInteract(PC);
	GetOwner()->HasAuthority()
		? Authority_OnInteract(Location, Rotation.Vector())
		: Server_DoInteract(Location, Rotation.Vector());
}

void UInteractComponent::Authority_OnInteract(const FVector& Pos, const FVector& Dir)
{
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bTraceComplex = false;
	Params.bDebugQuery = true;
	GetWorld()->LineTraceSingleByChannel(Result, Pos, Pos + (Dir * TraceLength), ECollisionChannel::ECC_GameTraceChannel10, Params);
	IInteractable* Interactable = Cast<IInteractable>(Result.GetActor());
	if (!Interactable) return;
	ACharacter* CA = GetOwner<ACharacter>();
	APlayerController* PC = CA->GetController<APlayerController>();
	DrawDebugLine(GetWorld(), Pos, Pos + (Dir * TraceLength), FColor::Cyan, true);
	if (Interactable->ShouldInteractOnAuthority())
		Interactable->Authority_OnInteract(PC);
}

void UInteractComponent::Server_DoInteract_Implementation(const FVector Pos, const FVector Dir)
{
	Authority_OnInteract(Pos,Dir);
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	
}