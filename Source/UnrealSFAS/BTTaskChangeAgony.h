// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UnrealSFASNPC.h"
#include "BTTaskChangeAgony.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UBTTaskChangeAgony : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTaskChangeAgony(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bAgonizing = false;
};
