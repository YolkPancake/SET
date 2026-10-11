// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Define/SETGameplayTag.h"
#include "LobbyController.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SET_API ALobbyController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

};
