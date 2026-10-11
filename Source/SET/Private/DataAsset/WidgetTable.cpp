// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/WidgetTable.h"


#include "UI/Base/SETWidgetBase.h"

UClass* UWidgetTable::GetWidgetClass(const FGameplayTag& WidgetType)
{
	TSoftClassPtr<USETWidgetBase>* CtxClass = SoftWidgetClassMap.Find(WidgetType);
	checkf(CtxClass != nullptr && !CtxClass->IsNull(), TEXT("UWidgetTable中不存在类型为%s的控件类,请设置"),*WidgetType.ToString());
	return CtxClass->LoadSynchronous();
}
