// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"

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


// Called to unpossess our NPC pawn (in death)
void AUnrealSFASNPC::UnPossessed()
{

	UE_LOG(LogTemp, Warning, TEXT("Head Imploded!"));

}

// Called to return the patrol path of current NPC
AUnrealSFASPatrolPath* AUnrealSFASNPC::GetPatrolPath() const
{
	return PatrolPath;
}


// Called to set the head scale of the NPC
void AUnrealSFASNPC::ScaleHeadSize(float NewHeadSize)
{
	

	HeadSize = FMath::Clamp((HeadSize+NewHeadSize), 1.0f, MaxHeadSize);

	if (HeadSize >= MaxHeadSize)
	{
		AController* SavedController = GetController();
		HeadSize = 0.f; // head being imploded
		SavedController->UnPossess(); //Unpossess the pawn
	}
		


}

// Called to get the head scale of the NPC
float AUnrealSFASNPC::GetHeadSize() const
{

	return HeadSize;

}




