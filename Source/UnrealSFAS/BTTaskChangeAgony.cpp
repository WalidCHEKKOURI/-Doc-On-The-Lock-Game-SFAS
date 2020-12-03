// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskChangeAgony.h"
#include "NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UnrealSFASNPC.h"


UBTTaskChangeAgony::UBTTaskChangeAgony(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Change NPC Agonizing State");
}


EBTNodeResult::Type UBTTaskChangeAgony::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	ANPCAIController* const NPCAIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(NPCAIController->GetPawn());

	
		NPC->SetIsAgonizing(bAgonizing);

		// finish with success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	


}