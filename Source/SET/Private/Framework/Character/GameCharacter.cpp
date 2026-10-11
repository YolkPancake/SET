// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Character/GameCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Framework/ActorComponent/PlayerQuickBarComponent.h"
#include "Framework/Controller/GameController.h"
#include "Subsystem/WidgetManager.h"
#include "Framework/ActorComponent/InteractComponent.h"
#include "Framework/ActorComponent/HeldItemComponent.h"

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AmrMesh"));
	InteractComp = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComp"));
	HeldItemComp = CreateDefaultSubobject<UHeldItemComponent>(TEXT("HeldItemComp"));
	HeldItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeldItemMeshComp"));

	Camera->bUsePawnControlRotation = true;

	Camera->SetupAttachment(GetRootComponent());
	ArmMesh->SetupAttachment(Camera);
	HeldItemMeshComp->SetupAttachment(Camera);

}

void AGameCharacter::HandlePlayerReady()
{


	OnPlayerReady.Broadcast();
}

void AGameCharacter::HandleGameStarted()
{


	OnGameStarted.Broadcast();
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	InputComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AGameCharacter::Do_Move);
	InputComp->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGameCharacter::Do_Look);
	InputComp->BindAction(IA_Jump, ETriggerEvent::Started, this, &AGameCharacter::Do_Jump);
	InputComp->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AGameCharacter::Stop_Jump);

	InputComp->BindAction(IA_SwitchQuickBar, ETriggerEvent::Started, this, &AGameCharacter::Do_SwitchQuickBar);
	InputComp->BindAction(IA_WidgetInput, ETriggerEvent::Started, this, &AGameCharacter::Do_WidgetInput);
	InputComp->BindAction(IA_Interact, ETriggerEvent::Started, this, &AGameCharacter::Do_Interact);
	InputComp->BindAction(IA_GameInput, ETriggerEvent::Started, this, &AGameCharacter::Do_PressGameInput);
	InputComp->BindAction(IA_GameInput, ETriggerEvent::Completed, this, &AGameCharacter::Do_ReleaseGameInput);
}

bool AGameCharacter::InitGamePC()
{
	if (!GamePC.Get())
	{
		GamePC = GetController<AGameController>();
		if (!GamePC.Get())
		{
			UE_LOG(LogTemp, Error, TEXT("%s,尝试Cast到AGameController失败"), UE_SOURCE_LOCATION);
			return false;
		}
	}
	return true;
}

const FGameplayTag AGameCharacter::GetWidgetInputKey(const int Index)
{
	if (Index == 0) return SETGameplayTag::WidgetInputKey::Esc;
	if (Index == 1) return SETGameplayTag::WidgetInputKey::Tab;
	if (Index == 2) return SETGameplayTag::WidgetInputKey::M;


	checkNoEntry();
	return FGameplayTag{};
}

const FGameplayTag AGameCharacter::GetGameInputKey(const int Index)
{
	if (Index == 0) return SETGameplayTag::GameInputKey::LB;
	if (Index == 1) return SETGameplayTag::GameInputKey::RB;
	if (Index == 2) return SETGameplayTag::GameInputKey::Q;
	if (Index == 3) return SETGameplayTag::GameInputKey::E;
	if (Index == 4) return SETGameplayTag::GameInputKey::R;

	checkNoEntry();
	return FGameplayTag{};
}

void AGameCharacter::Do_Move(const FInputActionValue& Value)
{
	const FVector2D Vec = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Vec.Y);
	AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Vec.X);
}

void AGameCharacter::Do_Look(const FInputActionValue& Value)
{
	const FVector2D Vec = Value.Get<FVector2D>();
	AddControllerYawInput(Vec.X);
	AddControllerPitchInput(Vec.Y);
}

void AGameCharacter::Do_Jump(const FInputActionValue& Value)
{
	Jump();
}

void AGameCharacter::Stop_Jump(const FInputActionValue & Value)
{
	StopJumping();
}

void AGameCharacter::Do_SwitchQuickBar(const FInputActionValue& Value)
{
	if (!QBComp.IsValid())
	{
		if (!InitGamePC()) return;
		QBComp = GamePC->GetQuickBarComponent();
	}
	const float Val = Value.Get<float>();
	const int Index = FMath::RoundToInt32(Val) - 1;
	QBComp->Request_SwitchQuickBar(Index);
}

void AGameCharacter::Do_WidgetInput(const FInputActionValue& Value)
{
	const float Val = Value.Get<float>();
	const int Index = FMath::RoundToInt32(Val) - 1;
	UWidgetManager* WidgetManager = UWidgetManager::Get(GetController<AGameController>());
	WidgetManager->DispatchInput(GetWidgetInputKey(Index));
}

void AGameCharacter::Do_Interact(const FInputActionValue& Value)
{
	InteractComp->DoOnceInteract();
}

void AGameCharacter::Do_PressGameInput(const FInputActionValue& Value)
{
	if (!QBComp.IsValid())
	{
		if (!InitGamePC()) return;
		QBComp = GamePC->GetQuickBarComponent();
	}
	const float Val = Value.Get<float>();
	const int Index = FMath::RoundToInt32(Val) - 1;
	QBComp->DispatchGameInput(GetGameInputKey(Index), SETGameplayTag::InputKeyState::Press);
}

void AGameCharacter::Do_ReleaseGameInput(const FInputActionValue & Value)
{
	if (!QBComp.IsValid())
	{
		if (!InitGamePC()) return;
		QBComp = GamePC->GetQuickBarComponent();
	}
	const float Val = Value.Get<float>();
	const int Index = FMath::RoundToInt32(Val) - 1;
	QBComp->DispatchGameInput(GetGameInputKey(Index), SETGameplayTag::InputKeyState::Release);
}

