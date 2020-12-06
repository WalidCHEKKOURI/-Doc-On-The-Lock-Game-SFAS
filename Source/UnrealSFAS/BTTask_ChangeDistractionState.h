// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_ChangeDistractionState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBTTask_ChangeDistractionState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ChangeDistractionState(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bDistracted = false;
};
