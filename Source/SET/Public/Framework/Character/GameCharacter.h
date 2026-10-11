// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/Character/GameCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Define/SETGameplayTag.h"
#include "GameCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnPlayerReady);
DECLARE_MULTICAST_DELEGATE(FOnGameStarted);

class UInputAction;
class UCameraComponent;
class UPlayerQuickBarComponent;
class UInteractComponent;
class AGameController;
class UStaticMeshComponent;
class UHeldItemComponent;

UCLASS(Abstract, Blueprintable)
class SET_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 给子控件监听
	FOnPlayerReady OnPlayerReady;
	FOnGameStarted OnGameStarted;

	// Controller调用,准备钩子
	void HandlePlayerReady();
	void HandleGameStarted();

public:
	AGameCharacter();
	USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; }
	UHeldItemComponent* GetHeldItemComponent() const { return HeldItemComp; }
	UStaticMeshComponent* GetHeldItemMeshComponent() const { return HeldItemMeshComp; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	const FGameplayTag GetWidgetInputKey(const int Index);
	const FGameplayTag GetGameInputKey(const int Index);
	bool InitGamePC();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInteractComponent> InteractComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UHeldItemComponent> HeldItemComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> HeldItemMeshComp;


protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<AGameController> GamePC;


protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UPlayerQuickBarComponent> QBComp;


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_Move;
	void Do_Move(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_Look;
	void Do_Look(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_Jump;
	void Do_Jump(const FInputActionValue& Value);
	void Stop_Jump(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_SwitchQuickBar;
	void Do_SwitchQuickBar(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_WidgetInput;
	void Do_WidgetInput(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_Interact;
	void Do_Interact(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Config | InputAction")
	TObjectPtr<UInputAction> IA_GameInput;
	void Do_PressGameInput(const FInputActionValue& Value);
	void Do_ReleaseGameInput(const FInputActionValue& Value);
};
