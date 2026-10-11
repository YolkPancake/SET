// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadTaskCollector.generated.h"

class ILoadTaskProvider;
class ULoadingScreenManager;


DECLARE_DELEGATE_TwoParams(FOnLoadDataTaskCompleteDelegat, bool, const FText&);


UCLASS()
class SET_API ULoadTaskCollector : public UGameInstanceSubsystem
{
	GENERATED_BODY()


protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;


public:
	void RunLoadDataTask(APlayerController* PC, FOnLoadDataTaskCompleteDelegat CallBack);
protected:
	FOnLoadDataTaskCompleteDelegat LoadDataTaskCompleteDelegat;


	void BeginRollPoll(APlayerController* PC);
	void ColletTarget(APlayerController* PC);
	void DoRollPoll();
	void OnRollPollTimeout();

	void NotfiyLoadDataTaskComplete();
	void ClearCache();

	UPROPERTY()
	FTimerHandle RollPoll_Handle;
	UPROPERTY()
	FTimerHandle RollPollTimeout_Handle;

	UPROPERTY()
	int32 TotalTargetNum = 0;


	TDoubleLinkedList<ILoadTaskProvider*> TargetList;


	UPROPERTY(Transient)
	TWeakObjectPtr<ULoadingScreenManager> LoadingScreenManager;





};
