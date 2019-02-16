// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EnSUnitAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class ELSKA_N_STRIO_API UEnSUnitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Power)
	FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(UEnSUnitAttributeSet, Power);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Velocity)
	FGameplayAttributeData Velocity;
	ATTRIBUTE_ACCESSORS(UEnSUnitAttributeSet, Velocity);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Cooldown)
	FGameplayAttributeData Cooldown;
	ATTRIBUTE_ACCESSORS(UEnSUnitAttributeSet, Cooldown);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Lifespan)
	FGameplayAttributeData Lifespan;
	ATTRIBUTE_ACCESSORS(UEnSUnitAttributeSet, Lifespan);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Vision)
	FGameplayAttributeData Vision;
	ATTRIBUTE_ACCESSORS(UEnSUnitAttributeSet, Vision);

	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Stack)
	FGameplayAttributeData Stack;
	ATTRIBUTE_ACCESSORS(UEnSUnitAttributeSet, Stack);
	
protected:
	UFUNCTION()
		virtual void OnRep_Power();

	UFUNCTION()
		virtual void OnRep_Velocity();

	UFUNCTION()
		virtual void OnRep_Cooldown();

	UFUNCTION()
		virtual void OnRep_Lifespan();

	UFUNCTION()
		virtual void OnRep_Vision();

	UFUNCTION()
		virtual void OnRep_Stack();
};
