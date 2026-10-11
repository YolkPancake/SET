// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/ActivationContext/InventoryHUDContext.h"

#include "CoreMinimal.h"
#include "UI/Base/WidgetActivationContext.h"
#include "InventoryHUDContext.generated.h"


class AContainerActor;

UCLASS()
class SET_API UInventoryHUDContext : public UWidgetActivationContext
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient)
	TWeakObjectPtr<AContainerActor> ThisContainerActor;

};
