// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Define/Struct/WidgetInputConfig.h"



#include "WidgetInputConfig.generated.h"


struct FSETInputMode : public FInputModeDataBase
{
	FSETInputMode(const FWidgetInputConfig& Config);

	bool bShouldShowMouseCursor = false;
	bool bShouldBreakGameInput = false;
	EMouseLockMode MouseLockMode = EMouseLockMode::LockAlways;

	virtual void ApplyInputMode(class FReply& SlateOperations, class UGameViewportClient& GameViewportClient) const override;
	virtual ~FSETInputMode() = default;
};


UENUM()
enum class EInputType : uint8
{
	Game,
	UI
};

USTRUCT()
struct FWidgetInputConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EInputType InputType = EInputType::Game;

	UPROPERTY(EditDefaultsOnly)
	bool bShouldShowMouseCursor = false;

	UPROPERTY(EditDefaultsOnly)
	EMouseLockMode MouseLockMode = EMouseLockMode::LockAlways;


	UPROPERTY(EditDefaultsOnly)
	bool bShouldBreakGameInput = false;

	virtual ~FWidgetInputConfig() = default;
};
