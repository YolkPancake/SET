// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Button/UniversalButton.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UniversalButton.generated.h"

class UButton;
class USizeBox;
class UTextBlock;


DECLARE_MULTICAST_DELEGATE(FOnUniversalButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnUniversalButtonHovered);
DECLARE_MULTICAST_DELEGATE(FOnUniversalButtonUnhovered);


UCLASS(Abstract, Blueprintable)
class SET_API UUniversalButton : public UUserWidget
{
	GENERATED_BODY()
	

public:
	FOnUniversalButtonClicked OnUniversalButtonClicked;
	FOnUniversalButtonHovered OnUniversalButtonHovered;
	FOnUniversalButtonUnhovered OnUniversalButtonUnhovered;

	void SetUniversalButtonText(const FText& BtnText);



protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();


	UFUNCTION()
	void OnBtnClicked_CallBack();
	UFUNCTION()
	void OnBtnHovered_CallBack();
	UFUNCTION()
	void OnBtnUnhovered_CallBack();


protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonTextBlock;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;

	UPROPERTY(EditAnywhere, Category = "Config")
	FText ButtonText;

	UPROPERTY(EditAnywhere, Category = "Config")
	FVector2D ButtonSize{ 300.0f,200.0f };
};
