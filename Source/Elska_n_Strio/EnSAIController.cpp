// Fill out your copyright notice in the Description page of Project Settings.

#include "EnSAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnSAIController::AEnSAIController(const FObjectInitializer & ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	if (AIPerceptionComponent) {
		FAISenseAffiliationFilter AffiliationFilter;
		AffiliationFilter.bDetectEnemies = true;
		AffiliationFilter.bDetectFriendlies = false;
		AffiliationFilter.bDetectNeutrals = false;

		UAISenseConfig_Sight* SightConfig = NewObject< UAISenseConfig_Sight>();
		SightConfig->SightRadius = 3000.f;
		SightConfig->LoseSightRadius = 3500.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->DetectionByAffiliation = AffiliationFilter;
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
		
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnSAIController::UpdatePerceivedActors);
	}
}

void AEnSAIController::UpdatePerceivedActors(const TArray<AActor*>& UpdatedActors)
{
}

void AEnSAIController::SetTeamId(int32 Team)
{
	SetGenericTeamId(FGenericTeamId(Team));
}
