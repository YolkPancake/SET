// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Common/Slot/HUD/QuickBarSlot.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "QuickBarSlot.generated.h"


class UTextBlock;
class UImage;
class UBorder;

UCLASS(Abstract, Blueprintable)
class SET_API UQuickBarSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitQuickBarSlot(const FInventorySlot& InvSlot, int NewIndex);
	void RefreshQuickBarSlot(const FInventorySlot& InvSlot);
	int GetSlotIndex() const;
	void SetSlotIndex(int NewIndex);


protected:
	void ClearVisualSlot();
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
	int SlotIndex = 0;

	UPROPERTY(EditAnywhere)
	FSlateBrush CommonBrush;

	UPROPERTY(EditAnywhere)
	FSlateBrush SelectedBrush;
};
