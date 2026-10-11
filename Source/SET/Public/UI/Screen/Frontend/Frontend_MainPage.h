// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/SETWidgetBase.h"
#include "Frontend_MainPage.generated.h"



class UUniversalButton;

UCLASS(Abstract, Blueprintable)
class SET_API UFrontend_MainPage : public USETWidgetBase
{
	GENERATED_BODY()
	




protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void OnCreateGameButtonClicked_CallBack();
	void OnJoinGameButtonClicked_CallBack();
	void OnExitGameButtonClicked_CallBack();

	void OnCreateSessionFailed_CallBack(const FText& Msg);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniversalButton> CreateGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniversalButton> JoinGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniversalButton> ExitGameButton;


};
