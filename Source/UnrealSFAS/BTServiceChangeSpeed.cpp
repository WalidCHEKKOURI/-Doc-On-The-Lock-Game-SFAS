// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceChangeSpeed.h"
#include "UnrealSFASNPC.h"
#include "NPCAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UnrealSFASCharacter.h"

UBTServiceChangeSpeed::UBTServiceChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed & Beep sound Pitch");
}

void UBTServiceChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const AIController = OwnerComp.GetAIOwner();
	AUnrealSFASNPC* const NPC = Cast<AUnrealSFASNPC>(AIController->GetPawn());
	NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;// change NPC Speed

	//Change beep audio of player to 1.f
	AUnrealSFASCharacter* Player = Cast<AUnrealSFASCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Speed > 150 ) Player->ChangeBeepAudioPitch(200.f);
	else Player->ChangeBeepAudioPitch(1.f);
}

FString UBTServiceChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NPC speed & Beep Sound Pitch");
}
