// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Interface/Interactable.h"

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"


class APlayerController;


UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};




class SET_API IInteractable
{
	GENERATED_BODY()

public:

	virtual void Local_OnInteract(APlayerController* InInstigator) {}
	virtual void Authority_OnInteract(APlayerController* InInstigator) {}
	virtual bool ShouldInteractOnLocal() const = 0;
	virtual bool ShouldInteractOnAuthority() const = 0;


};
