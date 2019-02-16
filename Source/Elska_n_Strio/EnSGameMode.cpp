// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EnSGameMode.h"
#include "EnSHUD.h"
#include "EnSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEnSGameMode::AEnSGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = AEnSHUD::StaticClass();
}
