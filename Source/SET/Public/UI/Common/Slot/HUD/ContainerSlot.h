// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Common/Slot/HUD/ContainerSlot.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "ContainerSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class AContainerActor;

UCLASS()
class SET_API UContainerSlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void InitContainerSlot(AContainerActor* InContainerActor,const FInventorySlot& InvSlot,int Index);
	void ResetContainerSlot();

	void RefreshContainerSlot(const FInventorySlot& InvSlot);

protected:
	void ClearSlot();

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> IconImg;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNumTextBlock;

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<AContainerActor> ContainerActor;

	UPROPERTY(Transient)
	int SlotIndex = 0;

	UPROPERTY(EditAnywhere)
	FSlateBrush CommonBrush;

	UPROPERTY(EditAnywhere)
	FSlateBrush SelectedBrush;
};
