// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceAffectPlayerTemperature.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBTServiceAffectPlayerTemperature : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTServiceAffectPlayerTemperature();

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	FString GetStaticServiceDescription() const override;
};
