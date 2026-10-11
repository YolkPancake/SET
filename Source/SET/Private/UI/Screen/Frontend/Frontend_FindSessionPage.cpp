// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screen/Frontend/Frontend_FindSessionPage.h"

#include "Subsystem/SessionManager.h"
#include "UI/Common/SessionInfoWidget.h"
#include "UI/Button/UniversalButton.h"
#include "Subsystem/WidgetManager.h"

#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"

void UFrontend_FindSessionPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ResearchButton->OnUniversalButtonClicked.AddUObject(this, &UFrontend_FindSessionPage::OnResearchButtonClicked_CallBack);
	BackButton->OnUniversalButtonClicked.AddUObject(this, &UFrontend_FindSessionPage::OnBackButtonClicked_CallBack);

}

FReply UFrontend_FindSessionPage::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		OnBackButtonClicked_CallBack();
	}
	return FReply::Handled();
}

void UFrontend_FindSessionPage::OnActivated(UWidgetActivationContext* Ctx)
{
	Super::OnActivated(Ctx);
	DoSearchSession();
}

void UFrontend_FindSessionPage::DoSearchSession()
{
	if (bIsSearchingSession)
		return;
	bIsSearchingSession = true;
	WidgetSwitcher->SetActiveWidgetIndex(1);
	USessionManager* SessionManager = USessionManager::Get(this);
	FFindSessionCompleteDelegate FindSessionCompleteCallBack;
	FindSessionCompleteCallBack.BindUObject(this, &UFrontend_FindSessionPage::OnFindSessionComplete_CallBack);

	// TODO: Test
	FSessionFindContext Ctx;
	SessionManager->FindSession(Ctx, FindSessionCompleteCallBack);
}

void UFrontend_FindSessionPage::OnFindSessionComplete_CallBack(bool bIsSuccess, const FText& Msg, const TArray<FSessionInformationData>& InfoData)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Msg.ToString());
	if (bIsSuccess)
	{
		InfoData.Num() > 0
			? RefreshSessionInfoList(InfoData)
			: WidgetSwitcher->SetActiveWidgetIndex(2);
	}
	bIsSearchingSession = false;
}

USessionInfoWidget* UFrontend_FindSessionPage::GetInfoWidget()
{
	if (AvailableInfoWidget.Num() > 0)
		return AvailableInfoWidget.Pop();
	USessionInfoWidget* NewInfoWidget = CreateWidget<USessionInfoWidget>(GetOwningPlayer(), InfoWidgetClass);
	UsedInfoWidget.Add(NewInfoWidget);
	return NewInfoWidget;
}

void UFrontend_FindSessionPage::ClearAllInfoWidget()
{
	while (UsedInfoWidget.Num()>0)
	{
		USessionInfoWidget* InfoWidget = UsedInfoWidget.Pop();
		InfoWidget->ResetSessionInfoWidget();
		AvailableInfoWidget.Add(InfoWidget);
	}
}

void UFrontend_FindSessionPage::RefreshSessionInfoList(const TArray<FSessionInformationData>& InfoData)
{
	ClearAllInfoWidget();
	for (int i = 0; i < InfoData.Num(); i++)
	{
		USessionInfoWidget* InfoWidget = GetInfoWidget();
		InfoWidget->InitSessionInfoWidget(InfoData[i],i);
		GridPanel->AddChildToUniformGrid(InfoWidget, i, 0);
	}
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UFrontend_FindSessionPage::OnResearchButtonClicked_CallBack()
{
	DoSearchSession();
}

void UFrontend_FindSessionPage::OnBackButtonClicked_CallBack()
{
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::Frontend::MainPage);
}

