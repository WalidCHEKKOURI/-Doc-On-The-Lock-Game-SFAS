// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealSFASPatrolPath.h"
#include "UnrealSFASNPC.generated.h"


UCLASS()
class UNREALSFAS_API AUnrealSFASNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnrealSFASNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// get the patrol path of this NPC
	AUnrealSFASPatrolPath* GetPatrolPath() const;


	// set the head size of this NPC
	UFUNCTION(BlueprintCallable, Category = "AI")
	void ScaleHeadSize(float NewHeadSize);

	UFUNCTION(BlueprintCallable, Category = "AI")
		float GetHeadSize() const;


	// Called to unpossess our NPC pawn
	virtual void UnPossessed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AUnrealSFASPatrolPath* PatrolPath;

	/* Character's head scale*/
	UPROPERTY()
	float HeadSize = 1;

	/* Character's Max head scale*/
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite ,Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MaxHeadSize = 4.f;

	/* Character's Min head scale*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MinHeadSize = 1.0f;


	
};
