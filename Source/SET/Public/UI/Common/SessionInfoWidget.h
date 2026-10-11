// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Common/SessionInfoWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/Struct/SessionInfoContext.h"
#include "SessionInfoWidget.generated.h"


class UTextBlock;
class UButton;

UCLASS(Abstract, Blueprintable)
class SET_API USessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void InitSessionInfoWidget(const FSessionInformationData& SessionInfo,const int Index);
	void ResetSessionInfoWidget();

protected:
	virtual void NativeOnInitialized() override;


	void OnJoinSessionFailed_CallBack(const FText& Msg);

	UFUNCTION()
	void OnJoinSessionButtonClicked_CallBack();

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> JoinSessionButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> SessionNameTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNumTextBlock;

protected:
	UPROPERTY(Transient)
	int SessionIndex = -1;
};
