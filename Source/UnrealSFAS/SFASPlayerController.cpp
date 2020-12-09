// Fill out your copyright notice in the Description page of Project Settings.


#include "SFASPlayerController.h"
#include "UnrealSFASNPC.h"
#include "Kismet/GameplayStatics.h"

/* Called to add to the total collected data */
void ASFASPlayerController::AddTotalCollectedData()
{

	TotalCollectedData++;


}

/* Called to return the total number of NPCs*/
int32 ASFASPlayerController::GetTotalNPCNumber() const
{
	TArray<AActor*> FoundNPCActors;
	auto classToFind = AUnrealSFASNPC::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, FoundNPCActors);

	return FoundNPCActors.Num();
}


