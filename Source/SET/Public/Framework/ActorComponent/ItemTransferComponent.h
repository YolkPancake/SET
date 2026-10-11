// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/ActorComponent/ItemTransferComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Storable.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "ItemTransferComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SET_API UItemTransferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemTransferComponent();

	void Request_TransferItem(const TScriptInterface<IStorable>& SourceStorable, const int32 SourceIndex, const TScriptInterface<IStorable>& TargetStorable, const int32 TargetIndex);

protected:
	UFUNCTION(Server, Reliable)
	void Server_TransferItem(const TScriptInterface<IStorable>& SourceStorable, const int32 SourceIndex, const TScriptInterface<IStorable>& TargetStorable, const int32 TargetIndex);

	void Authority_TransferItem(const TScriptInterface<IStorable>& SourceStorable, const int32 SourceIndex, const TScriptInterface<IStorable>& TargetStorable, const int32 TargetIndex);

	bool CanSwapItem(const FInventorySlot& SourceSlot, const FInventorySlot& TargetSlot);
};
