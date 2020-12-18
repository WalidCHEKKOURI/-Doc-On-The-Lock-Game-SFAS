// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GamePlayEffectTypes.h>
#include "Collectible.generated.h"

UCLASS(Blueprintable)
class UNREALSFAS_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();

protected:
	

	UFUNCTION()
	void ApplyEffect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* Root;

	//The volume of collision, we use this to detect overlaps with pawns, specifically with the player pawn
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Collison")
	class USphereComponent* CollisionVolume = nullptr;

	//the rotation rate of the collectible
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float RotationRate;

	/* Effects to apply*/
	UPROPERTY(BlueprintReadWrite, Category = "GAS", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TSubclassOf<class UGameplayEffect> EffectToApply;

	// if it's a battery collectible or a temperature collectible
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "CollectibleType", meta = (AllowPrivateAccess = "true", ExposeOnSpawn= "true") )
		bool bIsBattery = false;

public:

	// Particle Emitter spawned on overlap with player
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem", meta = (ExposeOnSpawn = "true"))
		UParticleSystem * OverlapParticleEmitter = nullptr;

};
