// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/ActorComponent/PlayerBagComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Storable.h"
#include "Interface/LoadTaskProvider.h"
#include "PlayerBagComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SET_API UPlayerBagComponent : public UActorComponent,public IStorable, public ILoadTaskProvider
{
	GENERATED_BODY()

public:	
	UPlayerBagComponent();
	virtual bool HasTheTaskBeenCompleted() override;
	virtual FInventoryList& GetInventory() override;
	virtual bool CanAcceptItem(const FGameplayTag& ItemType) override;
	virtual int32 GetInventorySize() const override;


	// 返回值 = 未能装下的数量(0 表示全部装入),因为有返回值,必须在权威端调用
	int Request_AddItem(const FInventorySlot& Slot);
	int Request_AddItem(const FInventoryItemData& ItemData);

protected:
	int Authority_DoAddItem(const FInventorySlot& Slot);
	int Authority_DoAddItem(const FInventoryItemData& ItemData);


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void NotifyInventoryItemChanged(const TArray<int32>& Indexs) override;



protected:
	UPROPERTY(Replicated)
	FInventoryList PlayerBag;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_bIsInventoryDataReady)
	bool bIsInventoryDataReady = false;

	UFUNCTION()
	void OnRep_bIsInventoryDataReady();




};
