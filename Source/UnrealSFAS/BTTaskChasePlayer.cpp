// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskChasePlayer.h"
#include "NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlackBoardKeys.h"
#include "UnrealSFASNPC.h"


UBTTaskChasePlayer::UBTTaskChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}


EBTNodeResult::Type UBTTaskChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get TargetLocation from blackboard via AI controller
	ANPCAIController* const NPCAIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	FVector const PlayerLocation = NPCAIController->GetBlackboardComp()->GetValueAsVector(bbKeys::target_location);

	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(NPCAIController->GetPawn());
	if (!NPC->GetIsDead())
	{
		// move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(NPCAIController, PlayerLocation);


		// finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	

	// finish with failure
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}