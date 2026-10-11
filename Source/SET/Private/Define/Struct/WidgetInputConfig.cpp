// Fill out your copyright notice in the Description page of Project Settings.


#include "Define/Struct/WidgetInputConfig.h"

FSETInputMode::FSETInputMode(const FWidgetInputConfig& Config)
{
	bShouldShowMouseCursor = Config.bShouldShowMouseCursor;
	bShouldBreakGameInput = Config.bShouldBreakGameInput;
	MouseLockMode = Config.MouseLockMode;
}

void FSETInputMode::ApplyInputMode(FReply& SlateOperations, UGameViewportClient& GameViewportClient) const
{
	TSharedPtr<SViewport> ViewportWidget = GameViewportClient.GetGameViewportWidget();
	if (ViewportWidget.IsValid())
	{
		if (bShouldShowMouseCursor)
		{
			GameViewportClient.SetMouseCaptureMode(EMouseCaptureMode::NoCapture);
			SlateOperations.ReleaseMouseCapture();
			GameViewportClient.SetMouseLockMode(MouseLockMode);
		}
		else
		{
			SlateOperations.SetUserFocus(ViewportWidget.ToSharedRef());
			GameViewportClient.SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
			GameViewportClient.SetMouseLockMode(EMouseLockMode::LockAlways);
			SlateOperations.LockMouseToWidget(ViewportWidget.ToSharedRef());
		}
	}
}
