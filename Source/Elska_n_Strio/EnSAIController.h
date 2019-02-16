// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnSAIController.generated.h"

/**
 * 
 */
UCLASS()
class ELSKA_N_STRIO_API AEnSAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

public:

	AEnSAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void UpdatePerceivedActors(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintCallable)
	void SetTeamId(int32 Team);

};
