// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskFindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALSFAS_API UBTTaskFindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTaskFindPlayerLocation(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowProtectedAccess = "true"))
		bool bSearchRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowProtectedAccess = "true"))
		float SearchRadius = 150.0f;
	
};
