// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSFASCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Animation/AnimInstance.h"
#include "DeathCauses.h"

#include "GASAbilitySystemComponent.h"
#include "MainCharacterAttributeSet.h"
#include <GameplayEffectTypes.h>
#include "Kismet/GameplayStatics.h"
#include "UnrealSFASNPC.h"
#include "Components/ArrowComponent.h"
#include "SFASPlayerController.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
//////////////////////////////////////////////////////////////////////////
// AUnrealSFASCharacter

AUnrealSFASCharacter::AUnrealSFASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// set front camera turn rate and bone rotation around axis
	FrontCameraTurnRate = 60.f;
	NeckArmRotationAroundAxis = 0.f;

	//set default minimum and maximum rotation around axis
	FrontCameraMaxTurnAroundAxis = 42.0f;
	FrontCameraMinTurnAroundAxis = -20.0f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a front facing camera
	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FrontCamera"));
	FrontCamera->SetupAttachment(GetMesh(), FName("FrontCameraSocket")); // Attach the camera to the FrontCameraSocket
	FrontCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	//Setup the strimulus component
	SetupStimulus();

	/*Ability System Component Setup*/
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);

	Attributes = CreateDefaultSubobject<UMainCharacterAttributeSet>(TEXT("Attributes"));

	//Setup sound component
	//BeepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioBeepComp"));
	//BeepAudioComponent->AttachParent = RootComponent;
	
		

}

// Called when the game starts or when spawned
void AUnrealSFASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Batter Energy starts to drain 
	ChangeBatteryEnergy();

	//Play beeping sound
	if(BeepSoundCue)
		BeepAudioComponent = UGameplayStatics::SpawnSound2D(this, BeepSoundCue);
	if (BeepAudioComponent && BeepSoundCue)
		BeepAudioComponent->Play();
	
	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealSFASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealSFASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealSFASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnFrontCamera", this, &AUnrealSFASCharacter::TurnFrontCamera);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnrealSFASCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnrealSFASCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealSFASCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealSFASCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnrealSFASCharacter::OnResetVR);

	// FlashLight effect input binding
	PlayerInputComponent->BindAction("FlashLight", IE_Pressed, this, &AUnrealSFASCharacter::ApplyFlashLight);

	// CollectData input binding
	PlayerInputComponent->BindAction("CollectData", IE_Pressed, this, &AUnrealSFASCharacter::CollectData);
	
}


void AUnrealSFASCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnrealSFASCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnrealSFASCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnrealSFASCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealSFASCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/* Returns Axis value of moving forward input */
float AUnrealSFASCharacter::GetMovingForwardAxisValue() const
{
	return MovingForwardAxisValue;
}


void AUnrealSFASCharacter::MoveForward(float Value)
{
	if (!bDead)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
		MovingForwardAxisValue = Value;
	}

	


}

void AUnrealSFASCharacter::MoveRight(float Value)
{
	if (!bDead)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
	
}

void AUnrealSFASCharacter::TurnFrontCamera(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		float newBoneRotationAroundAxis = NeckArmRotationAroundAxis + GetWorld()->GetDeltaSeconds() * FrontCameraTurnRate * Value;
		NeckArmRotationAroundAxis = FMath::Clamp(newBoneRotationAroundAxis, FrontCameraMinTurnAroundAxis, FrontCameraMaxTurnAroundAxis);

	}
}


void AUnrealSFASCharacter::SetupStimulus()
{

	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();

}

void AUnrealSFASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Setup the owner and the avatar, in this case it's the character for both
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
}


UAbilitySystemComponent* AUnrealSFASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AUnrealSFASCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		// The context in which we applying the effect
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);


		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			//Apply the effect to our main player character to initialize default values
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

}

void AUnrealSFASCharacter::Kill(TEnumAsByte<EDeathCauses> DeathType)
{
	if (!bDead)
	{
		ASFASPlayerController* PlayerSavedController = Cast<ASFASPlayerController>(GetController());
		bDead = true;
		bUseControllerRotationYaw = false;
		FTimerHandle handle;
		//Stop beeping sound
		BeepAudioComponent->Stop();
		//Spawn Death Emitter
		if (DeathParticleEmitter)
		{
			UE_LOG(LogTemp, Error, TEXT("Death particle system spawned !"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticleEmitter, GetActorTransform(), true);
		}

		else
			UE_LOG(LogTemp, Error, TEXT("Death particle system is null !"));

		switch (DeathType)
		{

		case EDeathCauses::NPCImplosion: // NPC Imploded before the player

			FollowCamera->Activate(true);
			FrontCamera->Activate(false);

			CharacterAnimInstance = GetMesh()->GetAnimInstance();
			if (DeathAnimMontage)
				CharacterAnimInstance->Montage_Play(DeathAnimMontage, 1.f);
			else
				UE_LOG(LogTemp, Error, TEXT("Death Montage is null !"));


			//Delay until the end of the animationMontage
			GetWorld()->GetTimerManager().SetTimer(handle, [this]()
				{
					GetMesh()->bPauseAnims = true; // pause the animation
					ASFASPlayerController* SavedController = Cast<ASFASPlayerController>(GetController());
					SavedController->OnDeath(EDeathCauses::NPCImplosion);
				}, (DeathAnimMontage->GetSectionLength(0) - 0.4), 0);


			break;

		case EDeathCauses::LowBatteryEnergy:
			GetMesh()->bPauseAnims = true; // pause the animation
			UE_LOG(LogTemp, Warning, TEXT("Deah by Low Battery"));
			
			PlayerSavedController->OnDeath(EDeathCauses::LowBatteryEnergy);
			break;

		case EDeathCauses::HighTemperature:
			GetMesh()->bPauseAnims = true; // pause the animation
			UE_LOG(LogTemp, Warning, TEXT("Deah by High Temperature"));
			
			PlayerSavedController->OnDeath(EDeathCauses::HighTemperature);

			break;



		default:
			break;
		}

		

	}
	
	
	

}

void AUnrealSFASCharacter::ChangeBatteryEnergy()
{
	
		FTimerHandle handle;
		//Loop every 3 seconds to decreaseBatterEnergy
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
			{

				if (!bDead)
				{

					float CurrentBatteryEnergy = Attributes->GetBatteryEnergy();


					if (AbilitySystemComponent && TimeBatteryEnergyEffect)
					{
						// The context in which we applying the effect
						FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
						EffectContext.AddSourceObject(this);


						FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TimeBatteryEnergyEffect, 1, EffectContext);
						if (SpecHandle.IsValid())
						{
							//Apply the effect to our main player character
							FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
							//Make sure to let the camera effect know that battery change is done
							OnBatteryEffectChanged();
						}
					}

					UE_LOG(LogTemp, Warning, TEXT("CurrentBatteryEnergy: %f"), CurrentBatteryEnergy);
					if (CurrentBatteryEnergy <= 0)
					{
						Kill(EDeathCauses::LowBatteryEnergy);
						return;
					}

				}
				else
					return;





			}, 3, 1);



	


	

}


void AUnrealSFASCharacter::ChangeTemperatureByAI()
{


	if (!bDead)
	{

		float CurrentTemperature = Attributes->GetTemperature();

		UE_LOG(LogTemp, Warning, TEXT("Current Temperature: %f"), CurrentTemperature);
		if (CurrentTemperature >= (MaxTemperature)) 
		{
			Kill(EDeathCauses::HighTemperature);//Killed by High Temperature
			return;
		}

		if (AbilitySystemComponent && TemperatureEffectByAI)
		{
			// The context in which we applying the effect
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);


			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(TemperatureEffectByAI, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				//Apply the effect to our main player character
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			
				//Inform BP on the temperature change
				OnTemperatureEffectChanged();
			}
		}

	

	}

}



float AUnrealSFASCharacter::GetTemperature() const
{

	return Attributes->GetTemperature();

}


float AUnrealSFASCharacter::GetBatteryEnergy() const
{

	return Attributes->GetBatteryEnergy();

}

void AUnrealSFASCharacter::ApplyFlashLight()
{
	UE_LOG(LogTemp, Warning, TEXT("Here 1FlashLight !"));
	//Get FlashLight Socket Location
	FVector SocketLocation = GetMesh()->GetSocketLocation("FlashLightSocket");
	FVector EndLocation = SocketLocation + (GetMesh()->GetRightVector() * -1.f * FlashLightReach);


	//Create the box shape we want to trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(30, 30, 30));
	FHitResult SweepResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	AUnrealSFASNPC* NPC = nullptr;
	if (GetWorld()->SweepSingleByChannel(SweepResult, SocketLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, Shape, CollisionParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("Here 2FlashLight !"));
		NPC = Cast<AUnrealSFASNPC>(SweepResult.GetActor());
		if (NPC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Here 3FlashLight !"));
			if (!NPC->GetDistracted())
			{
				float CurrentBatteryEnergy = Attributes->GetBatteryEnergy();
				UE_LOG(LogTemp, Warning, TEXT("Here 4FlashLight !"));
				//Flashlight can only be used if battery power is >= MinBatteryFlashLightPower: example 70.
				if (CurrentBatteryEnergy >= MinBatteryPowerForFlashLight)
				{
					//Distract NPC
					NPC->SetDistracted(true);
					FTransform Particletransform = GetActorTransform();
					Particletransform.SetLocation(GetActorLocation());
					if (CollectingDataParticleEmitter) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectingDataParticleEmitter, Particletransform, true);
					//Apply battery energy effect to decrease its energy
					ChangeBatteryEnergyByFlashLightEffect();
					UE_LOG(LogTemp, Warning, TEXT("FlashLight Distraction!"));
				}
			}
		}

	}




}

//Called to apply battery energy effect by flashlight
void AUnrealSFASCharacter::ChangeBatteryEnergyByFlashLightEffect()
{


	if (!bDead)
	{

		
			if (AbilitySystemComponent && FlashLightBatteryEffect)
			{
				// The context in which we applying the effect
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(this);


				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(FlashLightBatteryEffect, 1, EffectContext);
				if (SpecHandle.IsValid())
				{
					//Apply the effect to our main player character
					FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				}
			}

		
		

	}



}


//Called to collect data about NPC
void AUnrealSFASCharacter::CollectData()
{

	
	
	//Get FlashLight Socket Location
	FVector SocketLocation = GetMesh()->GetSocketLocation("FlashLightSocket");
	FVector EndLocation = SocketLocation + (GetMesh()->GetRightVector() * -1.f * FlashLightReach);


	//Create the box shape we want to trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(30, 30, 30));
	FHitResult SweepResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	AUnrealSFASNPC* NPC = nullptr;
	if (GetWorld()->SweepSingleByChannel(SweepResult, SocketLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, Shape, CollisionParams))
	{

		NPC = Cast<AUnrealSFASNPC>(SweepResult.GetActor());
		if (NPC)
		{
			
				//Vector of the trace
				FVector HitVector = (EndLocation - SocketLocation);
				float DotProductResult = FVector::DotProduct(NPC->GetActorForwardVector().GetSafeNormal(), HitVector.GetSafeNormal());
				if (NPC->CollectNPCData(DotProductResult))
				{
				



					if (bCanCollectData)
					{
						bCanCollectData = false;



						//Add score for total collected data
						ASFASPlayerController* SFASPlayerController = Cast<ASFASPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
						SFASPlayerController->AddTotalCollectedData();
						//Apply battery energy effect to decrease its energy
						ChangeBatteryEnergyByCollectingDataEffect();
					
						UE_LOG(LogTemp, Warning, TEXT("Collected Data!"));



						FTimerHandle handle;
						//Cooldown prior to reinitializing the CanCollectData variable
						GetWorld()->GetTimerManager().SetTimer(handle, [this]()
							{

								bCanCollectData = true;


							}, CollectDataCoolDown,01);

					}

				}
				
			

		}

	}


}


//Called to apply battery energy effect by collecting data
void AUnrealSFASCharacter::ChangeBatteryEnergyByCollectingDataEffect()
{


	if (!bDead)
	{


		if (AbilitySystemComponent && CollectingDataBatteryEffect)
		{
			// The context in which we applying the effect
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);


			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CollectingDataBatteryEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				//Apply the effect to our main player character
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			}
		}




	}



}

// Change beep audio pitch, called by NPC
void AUnrealSFASCharacter::ChangeBeepAudioPitch(float NewPitch)
{
//	if (BeepAudioComponent!= nullptr && BeepSoundCue != nullptr)
	//{
		//if(BeepAudioComponent->IsPlaying())
		//	BeepAudioComponent->SetPitchMultiplier(NewPitch);
	//}
		

}


