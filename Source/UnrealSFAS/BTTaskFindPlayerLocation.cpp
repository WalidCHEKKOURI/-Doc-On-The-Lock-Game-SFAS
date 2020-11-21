// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFindPlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NPCAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BlackBoardKeys.h"

UBTTaskFindPlayerLocation::UBTTaskFindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTaskFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	/* get player character and the NPC's controller */
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Controller = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	/* get player location to use as an origin */
	FVector const PlayerLocation = Player->GetActorLocation();
	if (bSearchRandom)
	{
		FNavLocation NavLocation;

		/* get the navigation system and generate a random location near the player */
		UNavigationSystemV1* const NavigationSyst = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavigationSyst->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, NavLocation, nullptr))
		{
			/* set target location to random generated location near the player */
			Controller->GetBlackboardComp()->SetValueAsVector(bbKeys::target_location, NavLocation.Location);
		}
	}
	else
	{
		/* set target location to player location */
		Controller->GetBlackboardComp()->SetValueAsVector(bbKeys::target_location, PlayerLocation);
	}


	/* finish with success */
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}