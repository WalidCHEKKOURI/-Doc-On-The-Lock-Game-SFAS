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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// get the patrol path of this NPC
	AUnrealSFASPatrolPath* GetPatrolPath() const;

	/// TODO Create a separate Service for headSize
	/// TODO Put Min & Max limits to bone scalar

	// get the head size of this NPC
	UFUNCTION(BlueprintCallable, Category = "AI")
	float GetHeadSize() const;

	// set the head size of this NPC
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetHeadSize(float NewHeadSize);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AUnrealSFASPatrolPath* PatrolPath;

	/* Character's head scale*/
	UPROPERTY()
	float HeadSize = 1;

	
};
