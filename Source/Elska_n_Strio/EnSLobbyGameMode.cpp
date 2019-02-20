// Fill out your copyright notice in the Description page of Project Settings.

#include "EnSLobbyGameMode.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include "EnSGameInstance.h"

void AEnSLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &AEnSLobbyGameMode::StartGame, 10);
	}
}

void AEnSLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void AEnSLobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UEnSGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/EnS/Maps/Inframundo?listen");
}
