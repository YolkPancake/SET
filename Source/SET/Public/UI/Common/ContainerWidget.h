// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Common/ContainerWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "ContainerWidget.generated.h"


class UContainerSlot;
class UInventoryHUDContext;
class UUniformGridPanel;
class AContainerActor;

UCLASS(Abstract, Blueprintable)
class SET_API UContainerWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void InitContainerWidget(UInventoryHUDContext* InCtx);
	void ResetContainerWidget();

protected:
	UContainerSlot* GetContainerSlot();


	void OnContainerItemChanged_CallBack(const TArray<int>& Indexs);
	FDelegateHandle ContainerItemChanged_Handle;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;


	UPROPERTY(Transient)
	TWeakObjectPtr<AContainerActor> ContainerActor;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UContainerSlot> ContainerSlotClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UContainerSlot>> ContainerSlotArray;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UContainerSlot>> AvailableSlotArray;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UContainerSlot>> UsedSlotArray;
};
