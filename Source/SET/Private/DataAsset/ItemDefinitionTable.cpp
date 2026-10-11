// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/ItemDefinitionTable.h"

 #include "DataAsset/ItemDefinition.h"

UItemDefinition* UItemDefinitionTable::GetItemDefinition(const FGameplayTag& ItemType)
{
	TSoftObjectPtr<UItemDefinition>* ItemDef = ItemDefMap.Find(ItemType);
	checkf(ItemDef && !ItemDef->IsNull(), TEXT("UItemDefinitionTable中不存在类别为%s的UItemDefinition"), *ItemType.ToString());
	return ItemDef->LoadSynchronous();
}
