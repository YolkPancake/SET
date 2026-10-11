// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/Actor/ContainerActor/ContainerActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Interface/Storable.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "ContainerActor.generated.h"

UCLASS()
class SET_API AContainerActor : public AActor, public IInteractable, public IStorable
{
	GENERATED_BODY()
	
public:	
	AContainerActor();

	virtual void Local_OnInteract(APlayerController* InInstigator) override;
	virtual void Authority_OnInteract(APlayerController* InInstigator) override;
	virtual bool ShouldInteractOnLocal() const override;
	virtual bool ShouldInteractOnAuthority() const override;

	virtual void NotifyInventoryItemChanged(const TArray<int32>& Indexs) override;
	virtual FInventoryList& GetInventory() override;
	virtual bool CanAcceptItem(const FGameplayTag& ItemType) override;
	virtual int32 GetInventorySize() const override;




public:
	void Local_OpenContainerHUD(APlayerController* InInstigator);
	void Local_RemoveContainerHUD(APlayerController* InInstigator);

	void Authority_RemoveViewer(APlayerController* InViewer);


protected:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<APlayerController>> Viewer;


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	UPROPERTY(Replicated)
	FInventoryList InvList;




};
