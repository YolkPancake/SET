// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/ActorComponent/RPCComponent.h"

#include "Framework/Actor/ContainerActor/ContainerActor.h"

URPCComponent::URPCComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URPCComponent::Client_RemoveContainerHUD_Implementation(AContainerActor* ThisContainer)
{
	if (!ThisContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("%s,客户端不存在对应的ContainerActor"),UE_SOURCE_LOCATION);
		return;
	}
	ThisContainer->Local_RemoveContainerHUD(GetOwner<APlayerController>());
}

void URPCComponent::Server_RemoveViewer_Implementation(AContainerActor* ThisContainer, APlayerController* InViewer)
{
	if (!ThisContainer || !InViewer)
	{
		UE_LOG(LogTemp, Error, TEXT("%s,服务端不存在客户端发起的ContainerActor或者Viewer"), UE_SOURCE_LOCATION);
		return;
	}
	ThisContainer->Authority_RemoveViewer(InViewer);
}

void URPCComponent::Client_OpenContainerHUD_Implementation(AContainerActor* ThisContainer)
{
	if (!ThisContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("位于服务器上的箱子容器发起了Clinet_OnPlayerInteractContainerAcotr,但没有在客户端上发现对应的ContainerAcotr,请检查"));
		return;
	}
	ThisContainer->Local_OpenContainerHUD(GetOwner<APlayerController>());
}



