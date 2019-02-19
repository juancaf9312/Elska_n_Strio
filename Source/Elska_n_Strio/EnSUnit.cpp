// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EnSUnit.h"
#include "EnSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "AbilitySystemComponent.h"
#include "Engine/DataTable.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnSUnitAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AEnSUnit

AEnSUnit::AEnSUnit()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	MeshArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MeshArm->SetupAttachment(FirstPersonCameraComponent);
	MeshArm->TargetArmLength = 0.f;
	MeshArm->bInheritPitch = false;
	MeshArm->bInheritRoll = false;

	GetMesh()->SetupAttachment(MeshArm);
	GetMesh()->SetOwnerNoSee(true);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.


	// Ability system initializtion
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UEnSUnitAttributeSet>(TEXT("AttributeSet"));
	SetReplicateMovement(true);
	SetReplicates(true);
}

void AEnSUnit::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (AbilitySystemComponent) {
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AEnSUnit::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	TeamId = FGenericTeamId((int8)Team);
}

void AEnSUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AbilitySystemComponent) {

		AddStartupGameplayAbilities();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (ensure(AttrDataTable)) {
			AttributeSet = Cast<UEnSUnitAttributeSet>(const_cast<UAttributeSet*> (AbilitySystemComponent->InitStats(AttributeSet->GetClass(), AttrDataTable)));
		}
	}
}

void AEnSUnit::AddStartupGameplayAbilities()
{
	if (ensure(AbilitySystemComponent)) {

		if (Role == ROLE_Authority) {

			for (TSubclassOf<UGameplayAbility>& Ability : GameplayAbilities) {
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEnSUnit::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	   
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AEnSUnit::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AEnSUnit::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEnSUnit::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEnSUnit::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEnSUnit::LookUpAtRate);
}

UAbilitySystemComponent * AEnSUnit::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AEnSUnit::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AEnSUnit::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AEnSUnit::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AEnSUnit::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEnSUnit::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
