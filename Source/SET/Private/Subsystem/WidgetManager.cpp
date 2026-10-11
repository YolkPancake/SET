// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/WidgetManager.h"

#include "UI/Base/SETWidgetBase.h"
#include "Setting/SETDataAccessor.h"
#include "DataAsset/WidgetTable.h"
#include "Define/Struct/WidgetInputConfig.h"
#include "UI/Base/DDVisualSlot.h"

UWidgetManager* UWidgetManager::Get(APlayerController* InCtxPC)
{
	ULocalPlayer* LocalPlayer = InCtxPC->GetLocalPlayer();
	checkf(IsValid(LocalPlayer),
		TEXT("尝试获取UWidgetManager, 但传入的PC无法获取当前的LocalPlayer!UWidgetManager仅服务本地玩家,避免在权威环境传入非LocalController!!!"));
	return LocalPlayer->GetSubsystem<UWidgetManager>();
}

void UWidgetManager::ActivateWidget(const FGameplayTag& WidgetType, UWidgetActivationContext* Ctx)
{
	if (CurrentWidget.Get())
	{
		CurrentWidget->OnDeactivated();
		CurrentWidget = nullptr;
	}

	CurrentWidget = GetWidget(WidgetType);
	CurrentWidget->OnActivated(Ctx);
	ApplyInputConfig();
}

APlayerController* UWidgetManager::GetCurrentPlayerController() const
{
	return GetLocalPlayer()->PlayerController;
}

void UWidgetManager::DispatchInput(const FGameplayTag& Inkey)
{
	if (CurrentWidget.Get())
		CurrentWidget->HandleInputKey(Inkey);
	UE_LOG(LogTemp, Error, TEXT("DespatchKey:%s"),*Inkey.ToString());
}

UDDVisualSlot* UWidgetManager::GetDDVisualSlot() const
{
	return DDVisualSlot.Get();
}

void UWidgetManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);



}

void UWidgetManager::Deinitialize()
{
	WidgetMap.Empty();
	if (CurrentWidget.Get())
		CurrentWidget->OnDeactivated();
	CurrentWidget = nullptr;
	DDVisualSlot = nullptr;


	Super::Deinitialize();
}

bool UWidgetManager::ShouldCreateSubsystem(UObject* Outer) const
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Outer);
	return !LocalPlayer->GetGameInstance()->IsDedicatedServerInstance();
}

void UWidgetManager::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);

	if (CurrentWidget.Get())
		CurrentWidget->OnDeactivated();
	CurrentWidget = nullptr;
	WidgetMap.Empty();
	DDVisualSlot = nullptr;


	UClass* DDVisualSlotClass = USETDataAccessor::Get()->GetDDVisualSlotClass();
	DDVisualSlot = CreateWidget<UDDVisualSlot>(NewPlayerController, DDVisualSlotClass);
}

USETWidgetBase* UWidgetManager::GetWidget(const FGameplayTag& WidgetType)
{
	if (TObjectPtr<USETWidgetBase>* CachedWidget = WidgetMap.Find(WidgetType))
		return CachedWidget->Get();
	UWidgetTable* WidgetTable = USETDataAccessor::Get()->GetWidgetTable();
	UClass* WidgetClass = WidgetTable->GetWidgetClass(WidgetType);
	USETWidgetBase* NewWidget = CreateWidget<USETWidgetBase>(GetLocalPlayer()->PlayerController, WidgetClass);
	NewWidget->RegisterWidgetManager(this);
	WidgetMap.Add(WidgetType, NewWidget);
	NewWidget->SetVisibility(ESlateVisibility::Collapsed);
	NewWidget->AddToViewport();
	return NewWidget;
}

void UWidgetManager::ApplyInputConfig()
{
	if (!IsValid(CurrentWidget.Get()))
		return;
	APlayerController* PC = GetLocalPlayer()->PlayerController;
	const FWidgetInputConfig& Config = CurrentWidget->GetRequiredInputConfig();
	PC->SetShowMouseCursor(Config.bShouldShowMouseCursor);
	switch (Config.InputType)
	{
		case EInputType::Game:
		{
			FInputModeGameOnly G;
			G.SetConsumeCaptureMouseDown(true);
			PC->SetInputMode(G);
			break;
		}

		case EInputType::UI:
		{
			FInputModeUIOnly U;
			U.SetLockMouseToViewportBehavior(Config.MouseLockMode);
			U.SetWidgetToFocus(CurrentWidget->GetCachedWidget());
			PC->SetInputMode(U);
			break;
		}
	}
	PC->FlushPressedKeys();
}
