// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/LoadTaskCollector.h"

#include "Interface/LoadTaskProvider.h"
#include "Subsystem/LoadingScreenManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"

bool ULoadTaskCollector::ShouldCreateSubsystem(UObject* Outer) const
{
	UGameInstance* GI = Cast<UGameInstance>(Outer);
	return !GI->IsDedicatedServerInstance();
}

void ULoadTaskCollector::RunLoadDataTask(APlayerController* PC, FOnLoadDataTaskCompleteDelegat CallBack)
{
	LoadDataTaskCompleteDelegat = CallBack;
	BeginRollPoll(PC);
}

void ULoadTaskCollector::BeginRollPoll(APlayerController* PC)
{
	ColletTarget(PC);
	UGameInstance* GI = GetGameInstance();
	if (!LoadingScreenManager.IsValid())
		LoadingScreenManager = GI->GetSubsystem<ULoadingScreenManager>();
	GI->GetTimerManager().SetTimer(
		RollPoll_Handle, this, &ULoadTaskCollector::DoRollPoll, 1, true
	);
	GI->GetTimerManager().SetTimer(
		RollPollTimeout_Handle, this, &ULoadTaskCollector::OnRollPollTimeout, 60, false
	);
}

void ULoadTaskCollector::ColletTarget(APlayerController* PC)
{
	TArray<AActor*> Core{ PC,PC->PlayerState,PC->GetPawn() };
	for (AActor* ActorTarget : Core)
	{
		if (ILoadTaskProvider* ActorTaskProvider = Cast<ILoadTaskProvider>(ActorTarget))
			TargetList.AddTail(ActorTaskProvider);
		const TSet<UActorComponent*> Comps = ActorTarget->GetComponents();
		for(UActorComponent* CompTarget : Comps)
			if (ILoadTaskProvider* CompTaskProvider = Cast<ILoadTaskProvider>(CompTarget))
				TargetList.AddTail(CompTaskProvider);
	}
	TotalTargetNum = TargetList.Num();
}

void ULoadTaskCollector::DoRollPoll()
{
	for (TDoubleLinkedList<ILoadTaskProvider*>::TDoubleLinkedListNode* Node = TargetList.GetHead();Node;)
	{
		ILoadTaskProvider* TaskProvider = Node->GetValue();
		TDoubleLinkedList<ILoadTaskProvider*>::TDoubleLinkedListNode* NextNode = Node->GetNextNode();
		if (!TaskProvider || TaskProvider->HasTheTaskBeenCompleted())
			TargetList.RemoveNode(Node);
		Node = NextNode;
	}
	const float Progress = (TotalTargetNum == 0)
		? 1.0f
		: 1.0f - static_cast<float>(TargetList.Num()) / TotalTargetNum;
	LoadingScreenManager->SetProgress(Progress);
	if (Progress >= 1.0f)
		NotfiyLoadDataTaskComplete();
}

void ULoadTaskCollector::OnRollPollTimeout()
{
	UGameInstance* GI = GetGameInstance();
	GI->GetTimerManager().ClearTimer(RollPoll_Handle);
	GI->GetTimerManager().ClearTimer(RollPollTimeout_Handle);
	FString Msg = FString::Printf(TEXT("检查数据加载超时(60s)!最终进度%f"), 1.0f - (static_cast<float>(TargetList.Num()) / TotalTargetNum));
	LoadDataTaskCompleteDelegat.ExecuteIfBound(false, FText::FromString(Msg));
	ClearCache();
}

void ULoadTaskCollector::NotfiyLoadDataTaskComplete()
{
	UGameInstance* GI = GetGameInstance();
	GI->GetTimerManager().ClearTimer(RollPoll_Handle);
	GI->GetTimerManager().ClearTimer(RollPollTimeout_Handle);
	LoadDataTaskCompleteDelegat.ExecuteIfBound(true, FText::FromString(TEXT("数据加载完毕")));
	ClearCache();
}

void ULoadTaskCollector::ClearCache()
{
	LoadDataTaskCompleteDelegat = FOnLoadDataTaskCompleteDelegat{};
	TotalTargetNum = 0;
	TargetList.Empty();
}
