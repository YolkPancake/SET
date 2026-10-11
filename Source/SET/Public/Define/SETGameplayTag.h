// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Define/SETGameplayTag.h"

#include "NativeGameplayTags.h"	



namespace SETGameplayTag::ItemType
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Wood);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stone);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rifle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shotgun);
}

namespace SETGameplayTag::ItemState
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CurrentNum);
}

namespace SETGameplayTag::ItemState::Weapon::Gun
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CurrentAmmo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxAmmo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ReloadTime);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireInterval);
}

namespace SETGameplayTag::ItemState::Weapon::MeleeWeapon
{

}

namespace SETGameplayTag::ItemTag
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Material);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Food);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon);
}

namespace SETGameplayTag::WidgetType::Frontend
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MainPage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(FindSessionPage);

}

namespace SETGameplayTag::WidgetType::HUD
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MainHUD);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryHUD);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ContainerHUD);
}

// 控件专属InputKey,不能与下面的GameInputKey存在相同按键!!!
// 控件只用控件的,游戏只用游戏的,不能混用!
namespace SETGameplayTag::WidgetInputKey
{
	// 0
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Esc);
	// 1
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tab);
	// 2
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(M);
}

namespace SETGameplayTag::GameInputKey
{
	// 0
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(LB);
	// 1
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(RB);
	// 2
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Q);
	// 3
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(E);
	// 4
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(R);
}

namespace SETGameplayTag::InputKeyState
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Press);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Release);
}