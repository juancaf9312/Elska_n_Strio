// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/NavigationObjectBase.h"
#include "EnSPlayerSpawnPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = Common, hidecategories = Collision)
class ELSKA_N_STRIO_API AEnSPlayerSpawnPoint : public ANavigationObjectBase
{
	GENERATED_BODY()

public:

	AEnSPlayerSpawnPoint(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Team;

	/** Arrow component to indicate forward direction of start */
#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
		class UArrowComponent* ArrowComponent;
public:
#endif

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetArrowComponent() const;
#endif
};
