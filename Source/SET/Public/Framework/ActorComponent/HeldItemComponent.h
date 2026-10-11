// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/ActorComponent/HeldItemComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeldItemComponent.generated.h"


class UPlayerQuickBarComponent;
class UStaticMeshComponent;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SET_API UHeldItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHeldItemComponent();


protected:
	virtual void BeginPlay() override;
	void OnHeldItemChanged_CallBack();
	void OnPlayerReady_CallBack();


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMeshComp;


protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UPlayerQuickBarComponent> QBComp;

};
