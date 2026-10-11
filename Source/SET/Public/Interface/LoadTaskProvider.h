// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Interface/LoadTaskProvider.h"

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LoadTaskProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULoadTaskProvider : public UInterface
{
	GENERATED_BODY()
};




class SET_API ILoadTaskProvider
{
	GENERATED_BODY()

public:
	virtual bool HasTheTaskBeenCompleted() = 0;


};
