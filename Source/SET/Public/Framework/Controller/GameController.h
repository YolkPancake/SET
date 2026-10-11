// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/Controller/GameController.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameController.generated.h"

class UInputMappingContext;
class UPlayerBagComponent;
class UPlayerQuickBarComponent;
class UItemTransferComponent;
class URPCComponent;

UCLASS(Abstract, Blueprintable)
class SET_API AGameController : public APlayerController
{
	GENERATED_BODY()
	


public:
	AGameController();
	UPlayerBagComponent* GetBagComponent() const;
	UPlayerQuickBarComponent* GetQuickBarComponent() const;
	UItemTransferComponent* GetItemTransferComponent() const;	
	URPCComponent* GetRPCComponent() const;

protected:
	virtual void BeginPlay() override;



protected:
	void OnGameStarted_CallBack(bool bIsSuccess, const FText& Msg);


	FTimerHandle CheckPlayerReady_THandle;

	void DoCheckPlayerReady();


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBagComponent> BagComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerQuickBarComponent> QuickBarComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemTransferComponent> ItemTransferComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<URPCComponent> RPCComp;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC_CA;
};
