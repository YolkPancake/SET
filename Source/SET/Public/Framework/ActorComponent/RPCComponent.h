// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Framework/ActorComponent/RPCComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPCComponent.generated.h"

/*
* 此组件给没有Onwer的世界Actor提供发RPC的功能
* 
*   核心就是给权威上Actor一条到与其互动的玩家的客户端的通道
*   通知客户端上的模拟代理干活
* 
* 
*/

/*
* 发RPC的前提是从Onwer链开始查找,找到一个PlayerConrtoller
* 拿到PlayerController上的NetConnection才能发RPC
* Actor的GetNetConnection默认从Onwer身上找
* 但对于世界中的掉落物,箱子等,他们不属于任何一个Player,强行设置Onwer也不好
* 这些东西被互动,都会得到一个对应互动的PC
* InteractComponent的互动请求一定是在权威发起的,
* 而有些操作需要本地执行,组件提供RPC能力
* 
*/


class AContainerActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SET_API URPCComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPCComponent();

	// Container Actor
public:
	UFUNCTION(Client,Reliable)
	void Client_OpenContainerHUD(AContainerActor* ThisContainer);

	UFUNCTION(Client,Reliable)
	void Client_RemoveContainerHUD(AContainerActor* ThisContainer);

	UFUNCTION(Server,Reliable)
	void Server_RemoveViewer(AContainerActor* ThisContainer,APlayerController* InViewer);

};
