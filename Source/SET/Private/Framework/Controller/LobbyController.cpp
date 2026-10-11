// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Controller/LobbyController.h"

#include "Subsystem/WidgetManager.h"

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();

	UWidgetManager* WidgetManager = UWidgetManager::Get(this);
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::Frontend::MainPage, nullptr);

}
