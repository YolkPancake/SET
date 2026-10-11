// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/UseActionManager.h"

#include "Object/ItemUseAction/ItemUseActionBase.h"

#include "Setting/SETDataAccessor.h"
#include "DataAsset/ItemDefinition.h"
#include "DataAsset/ItemDefinitionTable.h"


UUseActionManager* UUseActionManager::Get(UObject* InWorldCtxObj)
{
	if (GEngine)
		if (UWorld* World = GEngine->GetWorldFromContextObject(InWorldCtxObj, EGetWorldErrorMode::LogAndReturnNull))
			if (UGameInstance* Ins = World->GetGameInstance())
				return Ins->GetSubsystem<UUseActionManager>();
	return nullptr;
}

bool UUseActionManager::ShouldCreateSubsystem(UObject* Outer) const
{
	UGameInstance* Ins = Cast<UGameInstance>(Outer);
	return !Ins->IsDedicatedServerInstance();
}

UItemUseActionBase* UUseActionManager::GetItemUseAction(APlayerController* InLcoalPC,const FGameplayTag& InItemType)
{
	if (!InLcoalPC->IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("非LocalPC尝试请求UItemUseActionBase,拒绝!"));
		return nullptr;
	}
	UItemDefinitionTable* DefTable = USETDataAccessor::Get()->GetItemDefinitionTable();
	UItemDefinition* ItemDef = DefTable->GetItemDefinition(InItemType);
	if (ItemDef->UseActionClass.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("物品%s的UseAction为空,这是所希望的吗?"),*InItemType.ToString());
		return nullptr;
	}
	UClass* UseActionClass = ItemDef->UseActionClass.LoadSynchronous();
	return NewObject<UItemUseActionBase>(InLcoalPC, UseActionClass);
}
