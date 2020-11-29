// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceAffectPlayerTemperature.h"
#include "UnrealSFASCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UBTServiceAffectPlayerTemperature::UBTServiceAffectPlayerTemperature()
{

	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Player Temperature");


}


void UBTServiceAffectPlayerTemperature::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	/* get player character and the NPC's controller */
	AUnrealSFASCharacter* const PlayerCharacter = Cast<AUnrealSFASCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->ChangeTemperatureByAI();


}

FString UBTServiceAffectPlayerTemperature::GetStaticServiceDescription() const
{
	return FString("Change Player Temperature");
}
