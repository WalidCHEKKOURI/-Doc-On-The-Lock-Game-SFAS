// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTaskIncrementPathIndex.h"
#include "NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UnrealSFASNPC.h"
#include "BlackBoardKeys.h"
#include "Math/UnrealMathUtility.h"

UMyBTTaskIncrementPathIndex::UMyBTTaskIncrementPathIndex(FObjectInitializer const& ObjectInitializer)
{

	NodeName = TEXT("Increment Path Index");

}

EBTNodeResult::Type UMyBTTaskIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// get the AI controller
	ANPCAIController* const NPCAIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(NPCAIController->GetPawn());

	if (NPC->GetPatrolPath())
	{
		int const NbPoints = NPC->GetPatrolPath()->Num();
		if (NbPoints == -1)
		{
			//finish with failure
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
		int const MinIndex = 0;
		int const MaxIndex = NbPoints - 1;

		// get and set the black board index key
		int index = NPCAIController->GetBlackboardComp()->GetValueAsInt(bbKeys::patrol_path_index);
		if (bBidirectional) //change direction every time we reach the end of the path
		{
			if (index >= MaxIndex && DirectionEnum == EDirectionType::Forward)
			{
				DirectionEnum = EDirectionType::Reverse;
			}
			else if (index == MinIndex && DirectionEnum == EDirectionType::Reverse)
			{
				DirectionEnum = EDirectionType::Forward;
			}
		}
		NPCAIController->GetBlackboardComp()->SetValueAsInt(bbKeys::patrol_path_index,
			(DirectionEnum == EDirectionType::Forward ? FMath::Abs(++index) : FMath::Abs(--index)) % NbPoints);

		//finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		//finish with failure
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	

}