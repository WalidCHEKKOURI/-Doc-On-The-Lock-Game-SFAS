// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeDistractionState.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UnrealSFASNPC.h"
#include "NPCAIController.h"

UBTTask_ChangeDistractionState::UBTTask_ChangeDistractionState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Change NPC Distracted State");
}


EBTNodeResult::Type UBTTask_ChangeDistractionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ANPCAIController* const NPCAIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(NPCAIController->GetPawn());


	NPC->SetDistracted(bDistracted);

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;



}