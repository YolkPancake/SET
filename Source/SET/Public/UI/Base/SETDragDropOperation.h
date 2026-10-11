// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "UI/Base/SETDragDropOperation.h"

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Interface/Storable.h"
#include "SETDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class SET_API USETDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Transient)
	TScriptInterface<IStorable> SourceStorable;

	UPROPERTY(Transient)
	int SourceIndex;
};
