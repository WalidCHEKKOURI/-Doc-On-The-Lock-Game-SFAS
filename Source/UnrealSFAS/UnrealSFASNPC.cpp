// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASNPC.h"
#include "GameFramework/CharacterMovementComponent.h"


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

// Called to bind functionality to input
void AUnrealSFASNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called to return the patrol path of current NPC
AUnrealSFASPatrolPath* AUnrealSFASNPC::GetPatrolPath() const
{
	return PatrolPath;
}

// Called to return the head scale of the NPC
float AUnrealSFASNPC::GetHeadSize() const
{
	return HeadSize;
}

// Called to set the head scale of the NPC
void AUnrealSFASNPC::SetHeadSize(float NewHeadSize)
{

	HeadSize = HeadSize+NewHeadSize;
}





