// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Base/DDVisualSlot.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/Struct/SETInventoryDataDefine.h"
#include "DDVisualSlot.generated.h"


class UTextBlock;
class UImage;

UCLASS(Abstract, Blueprintable)
class SET_API UDDVisualSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RefreshDDVisualSlot(const FInventorySlot& InvSlot);



protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> IconImg;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNumTextBlock;
};
