// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnSGameMode.h"
#include "EnSLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ELSKA_N_STRIO_API AEnSLobbyGameMode : public AEnSGameMode
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

private:

	void StartGame();

	uint32 NumberOfPlayers = 0;

	FTimerHandle GameStartTimer;
};
