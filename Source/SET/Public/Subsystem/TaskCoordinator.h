// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Subsystem/TaskCoordinator.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Define/Struct/SessionInfoContext.h"
#include "TaskCoordinator.generated.h"


DECLARE_DELEGATE_TwoParams(FOnGameStartedDelegat,bool,const FText&)



UCLASS()
class SET_API UTaskCoordinator : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	static UTaskCoordinator* Get(const UObject* WorldCtxObj);


protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;



public:
	void RunLoadDataAndNotifyToStartGameTask(APlayerController* PC, FOnGameStartedDelegat CallBack);
protected:
	void OnLoadDataComplelt_CallBack(bool bIsSuccess, const FText& Msg);

	FOnGameStartedDelegat OnGameStartedDelegat;

};
