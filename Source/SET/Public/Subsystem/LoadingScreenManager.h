// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Subsystem/LoadingScreenManager.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenManager.generated.h"


class ULoadingScreen;

UCLASS()
class SET_API ULoadingScreenManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static ULoadingScreenManager* Get(UObject* WorldCtxObj);


protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


public:
	void BeginLoadMapProgress();

	void SetProgress(float NewProgress);
	void ReportLoadMapProgress(float NewProgress);
	void AddLoadingScreen();
	void RemoveLoadingScreen();


protected:
	UPROPERTY(Transient)
	TObjectPtr<ULoadingScreen> LoadingScreen;

protected:
	UPROPERTY(Transient)
	bool bIsLoadingScreenOnViewprot = false;
};
