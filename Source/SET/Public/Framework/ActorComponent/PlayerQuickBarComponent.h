// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/ActorComponent/PlayerQuickBarComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Storable.h"
#include "Interface/LoadTaskProvider.h"
#include "PlayerQuickBarComponent.generated.h"

class UItemUseActionBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentQuikcBarIndexChanged, const int&);
DECLARE_MULTICAST_DELEGATE(FOnHeldItemChanged);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SET_API UPlayerQuickBarComponent : public UActorComponent, public IStorable, public ILoadTaskProvider
{
	GENERATED_BODY()

public:
	FOnCurrentQuikcBarIndexChanged OnCurrentQuikcBarIndexChanged;
	FOnHeldItemChanged OnHeldItemChanged;


public:	
	UPlayerQuickBarComponent();
	virtual bool HasTheTaskBeenCompleted() override;
	int GetCurrentQuickBarIndex() const;
	virtual FInventoryList& GetInventory() override;
	virtual int32 GetInventorySize() const override;
	virtual bool CanAcceptItem(const FGameplayTag& ItemType) override;
	void DispatchGameInput(const FGameplayTag& InKey, const FGameplayTag& InState);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void NotifyInventoryItemChanged(const TArray<int32>& Indexs) override;



public:
	void Request_SwitchQuickBar(int NewIndex);

protected:
	void Authority_DoSwitchQuickBar(int NewIndex);

	UFUNCTION(Server,Reliable)
	void Server_DoSwitchQuickBar(int NewIndex);

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentQBIndex)
	int CurrentQBIndex = 0;

	UFUNCTION()
	void OnRep_CurrentQBIndex();


protected:
	UPROPERTY(Transient)
	TObjectPtr<UItemUseActionBase> CurrentUseAction;



protected:
	UPROPERTY(Replicated)
	FInventoryList PlayerQuickBar;
	
	UPROPERTY(Transient)
	FInventorySlot CachedInvSlot;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_bIsInventoryDataReady)
	bool bIsInventoryDataReady = false;

	UFUNCTION()
	void OnRep_bIsInventoryDataReady();
};
