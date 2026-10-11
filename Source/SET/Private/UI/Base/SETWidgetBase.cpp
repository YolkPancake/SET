// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/SETWidgetBase.h"

#include "Subsystem/WidgetManager.h"

void USETWidgetBase::OnActivated(UWidgetActivationContext* Ctx)
{
	SetVisibility(ESlateVisibility::Visible);
}

void USETWidgetBase::OnDeactivated()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void USETWidgetBase::RegisterWidgetManager(UWidgetManager* Manager)
{
	WidgetManager = Manager;
}

const FWidgetInputConfig& USETWidgetBase::GetRequiredInputConfig() const
{
	return InputConfig;
}
