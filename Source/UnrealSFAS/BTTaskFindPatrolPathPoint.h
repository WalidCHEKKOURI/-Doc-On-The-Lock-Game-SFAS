// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskFindPatrolPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBTTaskFindPatrolPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTaskFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
