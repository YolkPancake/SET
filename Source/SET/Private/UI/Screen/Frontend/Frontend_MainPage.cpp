// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Screen/Frontend/Frontend_MainPage.h"

#include "UI/Button/UniversalButton.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystem/WidgetManager.h"

#include "Subsystem/SessionManager.h"
#include "Subsystem/LoadingScreenManager.h"


void UFrontend_MainPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateGameButton->OnUniversalButtonClicked.AddUObject(this, &UFrontend_MainPage::OnCreateGameButtonClicked_CallBack);
	JoinGameButton->OnUniversalButtonClicked.AddUObject(this, &UFrontend_MainPage::OnJoinGameButtonClicked_CallBack);
	ExitGameButton->OnUniversalButtonClicked.AddUObject(this, &UFrontend_MainPage::OnExitGameButtonClicked_CallBack);
}

FReply UFrontend_MainPage::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		OnExitGameButtonClicked_CallBack();
	}
	return FReply::Handled();
}

void UFrontend_MainPage::OnCreateGameButtonClicked_CallBack()
{
	ULoadingScreenManager* LoadingScreenManager = ULoadingScreenManager::Get(this);
	LoadingScreenManager->AddLoadingScreen();
	LoadingScreenManager->BeginLoadMapProgress();
	USessionManager* SessionManager = USessionManager::Get(this);
	FCreateSessionFailedDelegate CreateSessionFailedCallBack;
	CreateSessionFailedCallBack.BindUObject(this, &UFrontend_MainPage::OnCreateSessionFailed_CallBack);
	// TODO: Test
	FSessionCreationContext Ctx;
	Ctx.MaxPlayerNum = 4;
	Ctx.ServerName = { TEXT("TestServer") };
	SessionManager->CreateSession(Ctx, CreateSessionFailedCallBack);
}

void UFrontend_MainPage::OnJoinGameButtonClicked_CallBack()
{
	WidgetManager->ActivateWidget(SETGameplayTag::WidgetType::Frontend::FindSessionPage, nullptr);
}

void UFrontend_MainPage::OnExitGameButtonClicked_CallBack()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit"));
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UFrontend_MainPage::OnCreateSessionFailed_CallBack(const FText& Msg)
{
	UE_LOG(LogTemp, Error, TEXT("%s"),*Msg.ToString());
	ULoadingScreenManager* LoadingScreenManager = ULoadingScreenManager::Get(this);
	LoadingScreenManager->RemoveLoadingScreen();
}
