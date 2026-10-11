// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Define/Struct/SessionInfoContext.h"

#include "SessionInfoContext.generated.h"


USTRUCT()
struct FSessionInformationData
{
	GENERATED_BODY()

	UPROPERTY()
	int MaxPlayerNum = 0;
	UPROPERTY()
	FString ServerName;
};


USTRUCT()
struct FSessionCreationContext
{
	GENERATED_BODY()


	UPROPERTY()
	FString ServerName{TEXT("")};

	UPROPERTY()
	int MaxPlayerNum = 0;
};

USTRUCT()
struct FSessionFindContext
{
	GENERATED_BODY()




};

USTRUCT()
struct FJoinSessionContext
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SessionIndex = 0;

};




USTRUCT()
struct FDestorySessionContext
{
	GENERATED_BODY()
};



