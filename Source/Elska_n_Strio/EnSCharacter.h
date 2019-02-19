// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "EnSCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AEnSCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* MeshArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Team;

	///------------------ Gameplay Ability System -----------------------------------------

protected:
	/** Our ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	class UEnSPlayerAttributeSet* AttributeSet;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> GameplayAbilities;

	///------------------------------------------------------------------------------------

	FGenericTeamId TeamId;

public:

	// Table with the attributes of the character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	class UDataTable * AttrDataTable;

	AEnSCharacter();
	virtual void OnRep_Controller() override;

protected:
	virtual void BeginPlay();

	virtual void PostInitializeComponents() override;

	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

protected:

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};

