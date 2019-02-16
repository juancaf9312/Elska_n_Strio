// Fill out your copyright notice in the Description page of Project Settings.

#include "EnSPlayerAttributeSet.h"
#include "UnrealNetwork.h"

UEnSPlayerAttributeSet::UEnSPlayerAttributeSet()
	:MovementVelocity(3.f)
	,Health(100)
	,Stamina(10.f)
{
}

void UEnSPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UEnSPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnSPlayerAttributeSet, MovementVelocity);
	DOREPLIFETIME(UEnSPlayerAttributeSet, Health);
	DOREPLIFETIME(UEnSPlayerAttributeSet, Stamina);
}

void UEnSPlayerAttributeSet::OnRep_MovementVelocity()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSPlayerAttributeSet, MovementVelocity);
}

void UEnSPlayerAttributeSet::OnRep_Health()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSPlayerAttributeSet, Health);
}

void UEnSPlayerAttributeSet::OnRep_Stamina()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSPlayerAttributeSet, Stamina);
}
