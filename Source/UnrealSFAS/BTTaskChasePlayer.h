// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UnrealSFASNPC.h"
#include "BTTaskChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBTTaskChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTaskChasePlayer(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
