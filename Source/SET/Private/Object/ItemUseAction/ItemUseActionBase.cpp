// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemUseAction/ItemUseActionBase.h"

void UItemUseActionBase::InitUseAction(APlayerController* InLocalPC)
{
	LocalPC = InLocalPC;

}

void UItemUseActionBase::ResetUseAction()
{
	LocalPC = nullptr;
}
