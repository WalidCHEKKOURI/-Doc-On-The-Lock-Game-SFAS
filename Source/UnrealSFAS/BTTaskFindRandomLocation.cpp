// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NPCAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BlackBoardKeys.h"


UBTTaskFindRandomLocation::UBTTaskFindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	/** set a meaningful name to the node  **/
	NodeName = TEXT("Find Random Location");

}

EBTNodeResult::Type UBTTaskFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/** get AI controller and its NPC **/
	auto const NPCAIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	auto const NPC = NPCAIController->GetPawn();

	/**  obtain NPC location to use as an origin location **/
	FVector const Origin =NPC->GetActorLocation();

	/** get the navigation system and generate a random location on the NavMesh **/
	UNavigationSystemV1* const NavigationSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation NavLocation;
	if (NavigationSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavLocation, nullptr))
	{

		NPCAIController->GetBlackboardComp()->SetValueAsVector(bbKeys::target_location, NavLocation.Location);
	}

	/** finish with success **/
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}


