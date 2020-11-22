// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceChangeSpeed.h"
#include "UnrealSFASNPC.h"
#include "NPCAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTServiceChangeSpeed::UBTServiceChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTServiceChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const AIController = OwnerComp.GetAIOwner();
	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(AIController->GetPawn());
	NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;// change NPC Speed
}

FString UBTServiceChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NPC speed");
}
