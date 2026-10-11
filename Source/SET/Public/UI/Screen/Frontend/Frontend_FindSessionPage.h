// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/SETWidgetBase.h"
#include "Define/Struct/SessionInfoContext.h"
#include "Frontend_FindSessionPage.generated.h"


/*
* 0Session展示界面
* 1搜索Session界面
* 2无Session信息界面
*/


class UUniversalButton;
class UWidgetSwitcher;
class USessionInfoWidget;
class UUniformGridPanel;


UCLASS(Abstract, Blueprintable)
class SET_API UFrontend_FindSessionPage : public USETWidgetBase
{
	GENERATED_BODY()
	


protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;


protected:
	virtual void OnActivated(UWidgetActivationContext* Ctx) override;

	void DoSearchSession();
	void OnFindSessionComplete_CallBack(bool bIsSuccess, const FText& Msg, const TArray<FSessionInformationData>& InfoData);
	USessionInfoWidget* GetInfoWidget();
	void ClearAllInfoWidget();
	void RefreshSessionInfoList(const TArray<FSessionInformationData>& InfoData);


	void OnResearchButtonClicked_CallBack();
	void OnBackButtonClicked_CallBack();


protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget)) 
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniversalButton> ResearchButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniversalButton> BackButton;


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USessionInfoWidget> InfoWidgetClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USessionInfoWidget>> AvailableInfoWidget;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USessionInfoWidget>> UsedInfoWidget;



protected:
	UPROPERTY(Transient)
	bool bIsSearchingSession = false;
};
