// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Subsystem/WidgetManager.h"

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Define/SETGameplayTag.h"
#include "WidgetManager.generated.h"


class UWidgetActivationContext;
class USETWidgetBase;
class UDDVisualSlot;



UCLASS()
class SET_API UWidgetManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	

public:
	static UWidgetManager* Get(APlayerController* InCtxPC);
	void ActivateWidget(const FGameplayTag& WidgetType, UWidgetActivationContext* Ctx = nullptr);
	APlayerController* GetCurrentPlayerController() const;
	void DispatchInput(const FGameplayTag& Inkey);
	UDDVisualSlot* GetDDVisualSlot() const;


protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	USETWidgetBase* GetWidget(const FGameplayTag& WidgetType);
	void ApplyInputConfig();

protected:
	UPROPERTY(Transient)
	TObjectPtr<USETWidgetBase> CurrentWidget;

	UPROPERTY(Transient)
	TObjectPtr<UDDVisualSlot> DDVisualSlot;

protected:
	// RuntimeCache
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<USETWidgetBase>> WidgetMap;
};
