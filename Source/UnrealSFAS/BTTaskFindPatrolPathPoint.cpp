// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFindPatrolPathPoint.h"
#include "NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UnrealSFASNPC.h"
#include "BlackBoardKeys.h"



UBTTaskFindPatrolPathPoint::UBTTaskFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{

	NodeName = TEXT("Find Patrol Path Point");

}


EBTNodeResult::Type UBTTaskFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// get the AI controller for the patrolling NPC
	ANPCAIController* const NPCAIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(NPCAIController->GetPawn());
	if (!NPC->GetIsDead())
	{
		// get the current patrol path index from the blackboard
		int const index = NPCAIController->GetBlackboardComp()->GetValueAsInt(bbKeys::patrol_path_index);

		// use the index to get the current patrol path from the NPC's reference to the patrol path
		
		FVector const PatrolPoint = NPC->GetPatrolPath()->GetPatrolPoint(index);

		// transform this point to a global position using its parent
		FVector const GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPoint);

		// write the current global path point to the blackboard
		NPCAIController->GetBlackboardComp()->SetValueAsVector(bbKeys::patrol_path_vector, GlobalPoint);

		// finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		// finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	


}

