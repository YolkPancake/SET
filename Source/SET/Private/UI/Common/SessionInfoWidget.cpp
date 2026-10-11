// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/SessionInfoWidget.h"

#include "Subsystem/TaskCoordinator.h"
#include "Subsystem/SessionManager.h"
#include "Subsystem/LoadingScreenManager.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionInfoWidget::InitSessionInfoWidget(const FSessionInformationData& SessionInfo, const int Index)
{
	SessionIndex = Index;
	SessionNameTextBlock->SetText(FText::FromString(SessionInfo.ServerName));
	PlayerNumTextBlock->SetText(FText::AsNumber(SessionInfo.MaxPlayerNum));
}

void USessionInfoWidget::ResetSessionInfoWidget()
{
	SessionIndex = -1;
}

void USessionInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	JoinSessionButton->OnClicked.AddDynamic(this, &USessionInfoWidget::OnJoinSessionButtonClicked_CallBack);
}

void USessionInfoWidget::OnJoinSessionFailed_CallBack(const FText& Msg)
{
	UE_LOG(LogTemp, Error, TEXT("%s"),*Msg.ToString());
}

void USessionInfoWidget::OnJoinSessionButtonClicked_CallBack()
{
	ULoadingScreenManager* LoadingScreenManager = ULoadingScreenManager::Get(this);
	LoadingScreenManager->AddLoadingScreen();
	USessionManager* SessionManager = USessionManager::Get(this);
	FJoinSessionFailedDelegate JoinSessionFaliedCallBack;
	JoinSessionFaliedCallBack.BindUObject(this, &USessionInfoWidget::OnJoinSessionFailed_CallBack);
	FJoinSessionContext Ctx;
	Ctx.SessionIndex = SessionIndex;
	SessionManager->JoinSession(Ctx, JoinSessionFaliedCallBack,GetOwningPlayer());
}
