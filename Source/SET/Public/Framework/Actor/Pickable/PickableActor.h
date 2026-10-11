// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "Interface/Interactable.h"
#include "PickableActor.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class UBillboardComponent;

UCLASS(Abstract, Blueprintable)
class SET_API APickableActor : public AActor,public IInteractable
{
	GENERATED_BODY()


public:	
	APickableActor();
	void Authority_InitPickableActor(const FInventoryItemData& Data);


	virtual void Authority_OnInteract(APlayerController* InInstigator) override;
	virtual bool ShouldInteractOnLocal() const override;
	virtual bool ShouldInteractOnAuthority() const override;

	// TODO :Test 后续由掉落逻辑下发,而不是编辑器手填
	UPROPERTY(EditAnywhere)
	FInventoryItemData TestData;


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnSphereAreaBeginOverlap_CallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereAreaEndOverlap_CallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	void Authority_OnInteracte(APlayerController* InInstigator);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereArea;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> Billboard;


protected:
	// 掉落物持有的就是一份"物品数据"(值语义),整体复制
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ItemData)
	FInventoryItemData ItemData;

	UFUNCTION()
	void OnRep_ItemData();





};
