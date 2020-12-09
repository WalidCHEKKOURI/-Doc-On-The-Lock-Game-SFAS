// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SFASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API ASFASPlayerController : public APlayerController
{
	GENERATED_BODY()
	

private:
	/* Number Of Total Collected Data */
	UPROPERTY()
	int32 TotalCollectedData = 0;

public:
	/* Called to return the total collected data */
	UFUNCTION(BlueprintCallable, Category = "Score")
		FORCEINLINE int32 GetTotalCollectedData() const { return TotalCollectedData; }

	/* Called to return the total number of NPCs*/
	UFUNCTION(BlueprintCallable, Category = "Score")
		int32 GetTotalNPCNumber() const;

	/* Called to add to the total collected data */
	UFUNCTION(BlueprintCallable, Category = "Score")
		void AddTotalCollectedData();
};
