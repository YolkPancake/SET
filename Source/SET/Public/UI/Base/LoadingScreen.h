// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Base/LoadingScreen.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"




UCLASS(Abstract, Blueprintable)
class SET_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void ReportLoadMapProgress(float NewProgress);
	void SetProgress(float NewProgress);




};
