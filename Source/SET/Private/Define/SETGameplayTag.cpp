// Fill out your copyright notice in the Description page of Project Settings.


#include "Define/SETGameplayTag.h"


// UE_DEFINE_GAMEPLAY_TAG( , "");

namespace SETGameplayTag::ItemType
{
	UE_DEFINE_GAMEPLAY_TAG(None, "SETGameplayTag.ItemType.None");
	UE_DEFINE_GAMEPLAY_TAG(Wood, "SETGameplayTag.ItemType.Wood");
	UE_DEFINE_GAMEPLAY_TAG(Stone, "SETGameplayTag.ItemType.Stone");
	UE_DEFINE_GAMEPLAY_TAG(Rifle, "SETGameplayTag.ItemType.Rifle");
	UE_DEFINE_GAMEPLAY_TAG(Shotgun, "SETGameplayTag.ItemType.Shotgun");
}

namespace SETGameplayTag::ItemState
{
	UE_DEFINE_GAMEPLAY_TAG(None, "SETGameplayTag.ItemState.None");
	UE_DEFINE_GAMEPLAY_TAG(CurrentNum, "SETGameplayTag.ItemState.CurrentNum");
}

namespace SETGameplayTag::ItemTag
{
	UE_DEFINE_GAMEPLAY_TAG(Material, "SETGameplayTag.ItemTag.Material");
	UE_DEFINE_GAMEPLAY_TAG(Food, "SETGameplayTag.ItemTag.Food");
	UE_DEFINE_GAMEPLAY_TAG(Weapon, "SETGameplayTag.ItemTag.Weapon");
}

namespace SETGameplayTag::WidgetType::Frontend
{
	UE_DEFINE_GAMEPLAY_TAG(MainPage, "SETGameplayTag.WidgetType.Frontend.MainPage");
	UE_DEFINE_GAMEPLAY_TAG(FindSessionPage, "SETGameplayTag.WidgetType.Frontend.FindSessionPage");
}

namespace SETGameplayTag::WidgetType::HUD
{
	UE_DEFINE_GAMEPLAY_TAG(MainHUD, "SETGameplayTag.WidgetType.HUD.MainHUD");
	UE_DEFINE_GAMEPLAY_TAG(InventoryHUD, "SETGameplayTag.WidgetType.HUD.InventoryHUD");
	UE_DEFINE_GAMEPLAY_TAG(ContainerHUD, "SETGameplayTag.WidgetType.HUD.ContainerHUD");
}

namespace SETGameplayTag::WidgetInputKey
{
	UE_DEFINE_GAMEPLAY_TAG(Esc, "SETGameplayTag.WidgetInputKey.Esc");
	UE_DEFINE_GAMEPLAY_TAG(Tab, "SETGameplayTag.WidgetInputKey.Tab");
	UE_DEFINE_GAMEPLAY_TAG(M, "SETGameplayTag.WidgetInputKey.M");
}

namespace SETGameplayTag::GameInputKey
{
	UE_DEFINE_GAMEPLAY_TAG(LB, "SETGameplayTag.GameInputKey.LB");
	UE_DEFINE_GAMEPLAY_TAG(RB, "SETGameplayTag.GameInputKey.RB");
	UE_DEFINE_GAMEPLAY_TAG(Q, "SETGameplayTag.GameInputKey.Q");
	UE_DEFINE_GAMEPLAY_TAG(E, "SETGameplayTag.GameInputKey.E");
	UE_DEFINE_GAMEPLAY_TAG(R, "SETGameplayTag.GameInputKey.R");
}

namespace SETGameplayTag::InputKeyState
{
	UE_DEFINE_GAMEPLAY_TAG(Press, "SETGameplayTag.InputKeyState.Press");
	UE_DEFINE_GAMEPLAY_TAG(Release, "SETGameplayTag.InputKeyState.Release");
}