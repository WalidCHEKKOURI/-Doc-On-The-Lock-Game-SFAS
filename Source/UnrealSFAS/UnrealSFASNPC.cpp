// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UnrealSFASCharacter.h"
#include "DeathCauses.h"
#include "NPCAIController.h"
#include "BlackBoardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AUnrealSFASNPC::AUnrealSFASNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Populate the DataCollected array with 0
	bDataCollected.Init(0, 2);
		
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
	//FRotator(0.f), FVector(1.f),
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImplosionParticleEmitter, GetMesh()->GetSocketLocation("HeadVirusSocket"), FRotator(1.f), FVector(1.f), true);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetMesh()->GetSocketLocation("HeadVirusSocket"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImplosionParticleEmitter, SpawnTransform, true);

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
		
		Player->Kill(EDeathCauses::NPCImplosion); //Kill player too
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




void  AUnrealSFASNPC::SetIsAgonizing(bool bNewAgonizingValue)
{
	bAgonizing = bNewAgonizingValue;
}

// Called to distract NPC if it's not already distracted
void  AUnrealSFASNPC::SetDistracted(bool isDistracted)
{
	
		
	ANPCAIController* SavedController = Cast<ANPCAIController>(GetController());
	if (isDistracted && !bDistracted)//Distract if not already distracted
	{
		bDistracted = isDistracted;
		
		SavedController->GetBlackboardComp()->SetValueAsBool(bbKeys::distracted, isDistracted);
	}	
	else if (!isDistracted)//Clear value
	{
		bDistracted = isDistracted;
		SavedController->GetBlackboardComp()->ClearValue(bbKeys::distracted);
	}
		
	
	
}

// Called to determine if data is already collected
bool AUnrealSFASNPC::bCanCollectData()
{
	
	return (bDataCollected[0] && bDataCollected[1]) ? false : true;


}

//Called to determine if data was collected successfully
bool AUnrealSFASNPC::CollectNPCData(float DotProductRes)
{

	UE_LOG(LogTemp, Error, TEXT("NPC Data collected %f"), DotProductRes);

	if (bCanCollectData())
	{
		if (DotProductRes < 0)//Front Side 
			bDataCollected[0] = 1;
		else //Back Side
			bDataCollected[1] = 1;

		return true;
	}
	else
		return false;

}

//Called to return the number of Data Collected, either 0,1 or 2
int AUnrealSFASNPC::CurrentCollectedData() const
{
	int Count = 0;

	for (bool element : bDataCollected)
		if (element) Count++;
	

	return Count;
}