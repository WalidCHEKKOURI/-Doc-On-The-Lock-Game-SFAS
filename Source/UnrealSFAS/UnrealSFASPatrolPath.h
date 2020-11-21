// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealSFASPatrolPath.generated.h"

UCLASS()
class UNREALSFAS_API AUnrealSFASPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnrealSFASPatrolPath();

	FVector GetPatrolPoint(int const index) const;
	int Num() const;

private:
	// array of patrolling points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector> PatrolPoints;

};
