// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/LoadingScreenManager.h"

#include "Setting/SETDataAccessor.h"
#include "UI/Base/LoadingScreen.h"

ULoadingScreenManager* ULoadingScreenManager::Get(UObject* WorldCtxObj)
{
	if (GEngine)
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldCtxObj, EGetWorldErrorMode::LogAndReturnNull))
			if (UGameInstance* Ins = World->GetGameInstance())
				return Ins->GetSubsystem<ULoadingScreenManager>();
	return nullptr;
}

bool ULoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	UGameInstance* GI = Cast<UGameInstance>(Outer);
	return !GI->IsDedicatedServerInstance();
}

void ULoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UClass* LoadingScreenClass = USETDataAccessor::Get()->GetLoadingScreenClass();
	LoadingScreen = CreateWidget<ULoadingScreen>(GetGameInstance(), LoadingScreenClass);
}

void ULoadingScreenManager::BeginLoadMapProgress()
{}

void ULoadingScreenManager::SetProgress(float NewProgress)
{
	LoadingScreen->SetProgress(NewProgress);
}

void ULoadingScreenManager::ReportLoadMapProgress(float NewProgress)
{
	LoadingScreen->ReportLoadMapProgress(NewProgress);
}

void ULoadingScreenManager::AddLoadingScreen()
{
	if (!bIsLoadingScreenOnViewprot)
	{
		UGameViewportClient* ViewportClient = GetGameInstance()->GetGameViewportClient();
		ViewportClient->AddViewportWidgetContent(LoadingScreen->TakeWidget(),100);
		bIsLoadingScreenOnViewprot = true;
	}
	LoadingScreen->SetVisibility(ESlateVisibility::Visible);
}

void ULoadingScreenManager::RemoveLoadingScreen()
{
	if (!bIsLoadingScreenOnViewprot)
		return;
	LoadingScreen->SetVisibility(ESlateVisibility::Collapsed);

}
