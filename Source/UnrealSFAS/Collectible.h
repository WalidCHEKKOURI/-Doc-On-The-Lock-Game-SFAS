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

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* OverlapParticleEmitter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Collison")
	class USphereComponent* CollisionVolume = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotationRate;

	/* Effects to apply*/
	UPROPERTY(BlueprintReadWrite, Category = "GAS")
	TSubclassOf<class UGameplayEffect> EffectToApply;

	// if it's a battery collectible or a temperature collectible
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "CollectibleType", meta = (AllowPrivateAccess = "true"))
		bool bIsBattery = false;
};
