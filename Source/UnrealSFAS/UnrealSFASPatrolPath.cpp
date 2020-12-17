// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASPatrolPath.h"

// Sets default values
AUnrealSFASPatrolPath::AUnrealSFASPatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


FVector AUnrealSFASPatrolPath::GetPatrolPoint(int const index) const
{

	return PatrolPoints[index];

}


int AUnrealSFASPatrolPath::Num() const
{
	return (PatrolPoints.Num()) ? PatrolPoints.Num() : -1;
}

void AUnrealSFASPatrolPath::SetPatrolPoints(TArray<FVector> NewPatrolPoints)
{
	PatrolPoints = NewPatrolPoints;

}