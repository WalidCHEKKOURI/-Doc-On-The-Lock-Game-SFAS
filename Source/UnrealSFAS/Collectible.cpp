// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include "Components/SphereComponent.h"
#include "GASAbilitySystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UnrealSFASCharacter.h"
#
// Sets default values
ACollectible::ACollectible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	//Setup CollisionVolume
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(FName("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	CollisionVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RotationRate = 100;
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::ApplyEffect);


}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	AddActorLocalRotation(FRotator(0, RotationRate * DeltaTime, 0));

}


void ACollectible::ApplyEffect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	

	AUnrealSFASCharacter* const Player = Cast<AUnrealSFASCharacter>(OtherActor);

	if (Player)
	{
		if (!Player->GetIsDead())
		{


			if (Player->GetAbilitySystemComponent() && EffectToApply)
			{
				// The context in which we applying the effect
				FGameplayEffectContextHandle EffectContext = Player->GetAbilitySystemComponent()->MakeEffectContext();
				EffectContext.AddSourceObject(this);


				FGameplayEffectSpecHandle SpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(EffectToApply, 1, EffectContext);
				if (SpecHandle.IsValid())
				{
					if ((bIsBattery && Player->GetBatteryEnergy() <= 80.f) || (!bIsBattery && Player->GetTemperature() >= 35.f))
					{


						//Apply the effect to our main player character
						FActiveGameplayEffectHandle ActiveGEHandle = Player->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

						//Spawn emitter
						FTransform SpawnTransform;
						SpawnTransform.SetLocation(GetActorLocation());
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticleEmitter, SpawnTransform, true);

						if (!bIsBattery)
						{
							//Inform BP on the temperature change
							Player->OnTemperatureEffectChanged();
						}

						Destroy();

					}

				}
			}




		}
	}
}

