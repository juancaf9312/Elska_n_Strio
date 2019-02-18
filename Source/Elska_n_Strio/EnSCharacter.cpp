// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "EnSCharacter.h"
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

#include "EnSPlayerAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AEnSCharacter

AEnSCharacter::AEnSCharacter()
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

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);

	OculusGoController = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OCulusGoController"));
	OculusGoController->SetupAttachment(R_MotionController);
	OculusGoController->SetOnlyOwnerSee(true);
	OculusGoController->bCastStaticShadow = false;
	OculusGoController->bCastDynamicShadow = false;

	bUsingMotionControllers = true;

	// Ability system initializtion
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UEnSPlayerAttributeSet>(TEXT("AttributeSet"));
	SetReplicateMovement(true);
	SetReplicates(true);
}

void AEnSCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (AbilitySystemComponent) {
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AEnSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	TeamId = FGenericTeamId((int8)Team);
}

void AEnSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AbilitySystemComponent) {

		AddStartupGameplayAbilities();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (ensure(AttrDataTable)) {
			AttributeSet = Cast<UEnSPlayerAttributeSet>(const_cast<UAttributeSet*> (AbilitySystemComponent->InitStats(AttributeSet->GetClass(), AttrDataTable)));
		}
	}
}

void AEnSCharacter::AddStartupGameplayAbilities()
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

void AEnSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	   
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AEnSCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AEnSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEnSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEnSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEnSCharacter::LookUpAtRate);
}

UAbilitySystemComponent * AEnSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AEnSCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AEnSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AEnSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AEnSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEnSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
