// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/UniversalButton.h"


#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


void UUniversalButton::SetUniversalButtonText(const FText& BtnText)
{
	ButtonTextBlock->SetText(BtnText);
}

void UUniversalButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn->OnClicked.AddDynamic(this, &UUniversalButton::OnBtnClicked_CallBack);
	Btn->OnHovered.AddDynamic(this, &UUniversalButton::OnBtnHovered_CallBack);
	Btn->OnUnhovered.AddDynamic(this, &UUniversalButton::OnBtnUnhovered_CallBack);


	ButtonTextBlock->SetText(ButtonText);
	SizeBox->SetWidthOverride(ButtonSize.X);
	SizeBox->SetHeightOverride(ButtonSize.Y);
}

void UUniversalButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		if (ButtonTextBlock)
		{
			ButtonTextBlock->SetText(ButtonText);
		}

		if (SizeBox)
		{
			SizeBox->SetWidthOverride(ButtonSize.X);
			SizeBox->SetHeightOverride(ButtonSize.Y);
		}
	}
}

void UUniversalButton::OnBtnClicked_CallBack()
{
	OnUniversalButtonClicked.Broadcast();
}

void UUniversalButton::OnBtnHovered_CallBack()
{
	OnUniversalButtonHovered.Broadcast();
}

void UUniversalButton::OnBtnUnhovered_CallBack()
{
	OnUniversalButtonUnhovered.Broadcast();
}

