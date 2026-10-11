// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Common/Slot/HUD/MainHUDVisualSlot.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "MainHUDVisualSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;

UCLASS(Abstract, Blueprintable)
class SET_API UMainHUDVisualSlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void InitVisualSlot(const FInventorySlot& InvSlot, int NewIndex);
	void RefreshVisualSlot(const FInventorySlot& InvSlot);
	int GetSlotIndex() const;
	void SetSlotIndex(int NewIndex);
	void SelectSlot();
	void UnseletSlot();

		
protected:
	void ClearVisualSlot();

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
