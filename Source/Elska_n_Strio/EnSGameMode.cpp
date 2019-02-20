// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EnSGameMode.h"
#include "EnSHUD.h"
#include "EnSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnSPlayerSpawnPoint.h"
#include "EnSCharacter.h"

AEnSGameMode::AEnSGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = AEnSHUD::StaticClass();
}

void AEnSGameMode::RespawnPlayer_Implementation(APlayerController* Player)
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnSPlayerSpawnPoint::StaticClass(), SpawnPoints);

	if (SpawnPoints.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player spawn points detected"));
		return;
	}
	   
	if (!Player)return;

	AEnSCharacter* PlayerCharacter = Cast<AEnSCharacter>(Player->GetPawn());
	if (PlayerCharacter)
	{
		int32 SpawnPintID = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);

		FTransform PointTransform = SpawnPoints[SpawnPintID]->GetActorTransform();
		SpawnPoints.RemoveAt(SpawnPintID);

		PlayerCharacter->SetActorTransform(PointTransform);

	}
}


bool AEnSGameMode::RespawnPlayer_Validate(APlayerController* Player)
{
	return Player!=nullptr;
}
