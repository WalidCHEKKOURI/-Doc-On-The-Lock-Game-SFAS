// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASMaze.h"
#include "NavigationSystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture.h"

// Sets default values
AUnrealSFASMaze::AUnrealSFASMaze()
{
 	// No need to tick the maze at this point
	PrimaryActorTick.bCanEverTick = false;


	

}

// Called when the game starts or when spawned
void AUnrealSFASMaze::BeginPlay()
{
	Super::BeginPlay();



	if (WallMesh && ExampleMaterial && WallMesh)
	{

		//Setup Wall Mesh Texture
		WallMesh->SetMaterial(0, ExampleMaterial);
		UMaterialInstanceDynamic* CubeMeshDynamicMaterial = UMaterialInstanceDynamic::Create(ExampleMaterial, WallMesh);
		WallMesh->SetMaterial(0, CubeMeshDynamicMaterial);
		CubeMeshDynamicMaterial->SetTextureParameterValue(TEXT("Texture"), WallTexture);


		const float blockSize = 200.0f;
		const float blockWidth = 2.0f;
		const float blockHeight = 5.0f;
		const float blockZPos = 50.0f;





		float xPos = 0.0f;
		float yPos = 0.0f;
		FQuat worldRotation(FVector(0.0f, 0.0f, 1.0f), 0.0f);
		FVector worldScale(blockWidth, blockWidth, blockHeight);
		uint32 mazeRow;

		USceneComponent* rootComponent = GetRootComponent();

		// Loop through the binary values to generate the maze as static mesh components attached to the root of this actor
		for (int32 i = 0; i < mazeSize; i++)
		{
			yPos = static_cast<float>(i - (mazeSize / 2)) * blockSize;

			//Select Maze based on chosen MazeLevel
			switch (MazeLevel)
			{
			case 1:
				mazeRow = mazeArray1[i];
				break;

			case 2:
				mazeRow = mazeArray2[i];
				break;

			case 3:
				mazeRow = mazeArray3[i];
				break;

			default:
				mazeRow = mazeArray1[i];
			}
			

			for (int32 j = 0; j < mazeSize; j++)
			{
				xPos = static_cast<float>(j - (mazeSize / 2)) * blockSize;

				uint32 mazeValue = (mazeRow >> (mazeSize - (j + 1))) & 1;

				if (mazeValue)
				{
					UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this);
					FVector worldPosition(xPos, yPos, blockZPos);
					FTransform worldXForm(worldRotation, worldPosition, worldScale);

					meshComponent->SetStaticMesh(WallMesh);
					meshComponent->SetWorldTransform(worldXForm);
					meshComponent->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepWorldTransform);
					meshComponent->RegisterComponent();
				}
			}
		}

	}

	// Rebuild navigation mesh
	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	NavSystem->Build();
}



