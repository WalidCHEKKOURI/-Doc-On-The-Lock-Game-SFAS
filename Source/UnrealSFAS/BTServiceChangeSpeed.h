// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBTServiceChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTServiceChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	FString GetStaticServiceDescription() const override;

private:
	// NPC speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float Speed = 300.0f;
};
