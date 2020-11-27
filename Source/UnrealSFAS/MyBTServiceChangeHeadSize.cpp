// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTServiceChangeHeadSize.h"
#include "UnrealSFASNPC.h"
#include "NPCAIController.h"

UMyBTServiceChangeHeadSize::UMyBTServiceChangeHeadSize()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change HeadSize");
}

void UMyBTServiceChangeHeadSize::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const AIController = OwnerComp.GetAIOwner();
	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(AIController->GetPawn());
	NPC->ScaleHeadSize(HeadSize);
}

FString UMyBTServiceChangeHeadSize::GetStaticServiceDescription() const
{
	return FString("Change the NPC speed");
}
