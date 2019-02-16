// Fill out your copyright notice in the Description page of Project Settings.

#include "EnSUnitAttributeSet.h"
#include "UnrealNetwork.h"

void UEnSUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UEnSUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UEnSUnitAttributeSet, Power);
	DOREPLIFETIME(UEnSUnitAttributeSet, Velocity);
	DOREPLIFETIME(UEnSUnitAttributeSet, Cooldown);
	DOREPLIFETIME(UEnSUnitAttributeSet, Lifespan);
	DOREPLIFETIME(UEnSUnitAttributeSet, Vision);
	DOREPLIFETIME(UEnSUnitAttributeSet, Stack);
}

void UEnSUnitAttributeSet::OnRep_Power()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSUnitAttributeSet, Power);
}

void UEnSUnitAttributeSet::OnRep_Velocity()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSUnitAttributeSet, Velocity);
}

void UEnSUnitAttributeSet::OnRep_Cooldown()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSUnitAttributeSet, Cooldown);
}

void UEnSUnitAttributeSet::OnRep_Lifespan()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSUnitAttributeSet, Lifespan);
}

void UEnSUnitAttributeSet::OnRep_Vision()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSUnitAttributeSet, Vision);
}

void UEnSUnitAttributeSet::OnRep_Stack()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnSUnitAttributeSet, Stack);
}
