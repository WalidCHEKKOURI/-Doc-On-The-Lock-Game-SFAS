// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MainCharacterAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class UNREALSFAS_API UMainCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
		UMainCharacterAttributeSet();

		/* Attribute of Temperature */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Temperature;
	ATTRIBUTE_ACCESSORS(UMainCharacterAttributeSet, Temperature);

	/* Attribute of BatteryEnergy */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData BatteryEnergy;
	ATTRIBUTE_ACCESSORS(UMainCharacterAttributeSet, BatteryEnergy);

};
