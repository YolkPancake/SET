// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/SETDataAccessor.h"

#include "DataAsset/ItemDefinitionTable.h"
#include "DataAsset/WidgetTable.h"
#include "UI/Base/LoadingScreen.h"
#include "UI/Base/DDVisualSlot.h"

USETDataAccessor* USETDataAccessor::Get()
{
	return GetMutableDefault<USETDataAccessor>();
}

UItemDefinitionTable* USETDataAccessor::GetItemDefinitionTable()
{
	checkf(!SoftItemtDefTable.IsNull(), TEXT("没有在USETDataAccessor设置SoftItemtDefTable"));
	return SoftItemtDefTable.LoadSynchronous();
}

UWidgetTable* USETDataAccessor::GetWidgetTable()
{	
	checkf(!SoftWidgetTable.IsNull(), TEXT("USETDataAccessor中没有设置SoftWidgetTable"));
	return SoftWidgetTable.LoadSynchronous();
}

UClass* USETDataAccessor::GetLoadingScreenClass()
{
	checkf(!LoadingScreenClass.IsNull(), TEXT("USETDataAccessor中没有设置LoadingScreenClass"));
	return LoadingScreenClass.LoadSynchronous();
}

UClass* USETDataAccessor::GetDDVisualSlotClass()
{
	checkf(!DDVisualSlotClass.IsNull(), TEXT("USETDataAccessor中没有设置DDVisualSlotClass"));
	return DDVisualSlotClass.LoadSynchronous();
}
