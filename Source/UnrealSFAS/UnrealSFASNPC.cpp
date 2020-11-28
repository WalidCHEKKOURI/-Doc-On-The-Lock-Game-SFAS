// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UnrealSFASCharacter.h"

// Sets default values
AUnrealSFASNPC::AUnrealSFASNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void AUnrealSFASNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnrealSFASNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to kill NPC
void AUnrealSFASNPC::KillNPC()
{
	
	HeadSize = 0.f; // head being imploded
	NPCAnimInstance = GetMesh()->GetAnimInstance();

	if (NPCAnimInstance)
	{
		FTimerHandle handle;
	
		bDead = true;
		if(DeathAnimMontage)
			NPCAnimInstance->Montage_Play(DeathAnimMontage, 1.f);
		else
			UE_LOG(LogTemp, Error, TEXT("Death Montage is null !"));

		/* get player character and the NPC's controller */
		AUnrealSFASCharacter* const Player = Cast<AUnrealSFASCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		Player->Kill(); //Kill player too
		UE_LOG(LogTemp, Warning, TEXT("Head Imploded !"));

		//Delay until the end of the animationMontage
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
			{
				
				
				GetMesh()->bPauseAnims = true; // pause the animation

				AController* SavedController =GetController();
				

				SavedController->UnPossess(); //Unpossess the pawn

			}, (DeathAnimMontage->GetSectionLength(0)-0.4), 0);



		
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error finding AnimInstance!"));
	}


}

// Called to return the patrol path of NPC
AUnrealSFASPatrolPath* AUnrealSFASNPC::GetPatrolPath() const
{
	return PatrolPath;
}

// Called to return if NPC is dead
bool AUnrealSFASNPC::GetIsDead() const
{

	return bDead;

}

// Called to set the head scale of the NPC
void AUnrealSFASNPC::ScaleHeadSize(float NewHeadSize)
{
	if (!bDead)
	{
		HeadSize = FMath::Clamp((HeadSize + NewHeadSize), 1.0f, MaxHeadSize);

		if (HeadSize >= MaxHeadSize)
		{
			KillNPC();
		}

	}
	


}

// Called to get the head scale of the NPC
float AUnrealSFASNPC::GetHeadSize() const
{

	return HeadSize;

}




