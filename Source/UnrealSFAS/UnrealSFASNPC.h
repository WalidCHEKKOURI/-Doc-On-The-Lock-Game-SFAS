// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealSFASPatrolPath.h"
//#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
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

	//Returns Head Size of NPC
	UFUNCTION(BlueprintCallable, Category = "AI")
	float GetHeadSize() const;

	//Returns if NPC is dead
	bool GetIsDead() const;


	//Called to set the agonizing state of NPC
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetIsAgonizing(bool bNewAgonizingValue);

	//Called to set the distracted state of NPC
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetDistracted(bool isDistracted);

	//Called to collect data successfully or not
	bool CollectNPCData(float DotProductRes);

	//Called to return the number of Data Collected, either 0,1 or 2
	UFUNCTION(BlueprintCallable, Category = "AI")
	int CurrentCollectedData() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AUnrealSFASPatrolPath* PatrolPath = nullptr;

	/* Character's head scale*/
	UPROPERTY()
	float HeadSize = 1;

	/* Character's Max head scale*/
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite ,Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MaxHeadSize = 4.f;

	/* Character's Min head scale*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MinHeadSize = 1.0f;

	/* Character's death animation montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DeathAnimMontage = nullptr;

	/* Character's AnimationInstance*/
	UPROPERTY()
	class UAnimInstance* NPCAnimInstance = nullptr;

	/*  Called to Kill NPC pawn*/
	void KillNPC();

	/* Is character dead */
	UPROPERTY()
	bool bDead = false;

	/* Is character distracted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bDistracted = false;

	/* Is Emmitter for the implosion particle effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImplosionParticleEmitter = nullptr;

	/* Is in agony */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bAgonizing = false;

	//Collected data from front and back of NPC, index 0 -> front, index 1 -> back
	UPROPERTY()
		TArray<bool> bDataCollected;

	// Called to determine if data is already collected
	bool bCanCollectData();

	
};
