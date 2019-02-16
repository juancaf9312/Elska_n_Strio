// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EnSHUD.generated.h"

UCLASS()
class AEnSHUD : public AHUD
{
	GENERATED_BODY()

public:
	AEnSHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:

};

