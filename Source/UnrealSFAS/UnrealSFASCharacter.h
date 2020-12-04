// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DeathCauses.h"
#include "AbilitySystemInterface.h"
#include <GamePlayEffectTypes.h>
#include "UnrealSFASCharacter.generated.h"


UCLASS(config=Game)
class AUnrealSFASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** FrontFacing camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FrontCamera;

	/** Neck and Arm rotation around axis */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float NeckArmRotationAroundAxis;

	/** Front camera turn rate */
	UPROPERTY()
	float FrontCameraTurnRate;

	/** Front camera maximum rotation around an axis */
	UPROPERTY()
		float FrontCameraMaxTurnAroundAxis;

	/** Front camera minimum rotation around an axis */
	UPROPERTY()
		float FrontCameraMinTurnAroundAxis;

	/** GAS component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
		class UGASAbilitySystemComponent* AbilitySystemComponent;

	/** GAS AttributeSet */
	UPROPERTY()
		class UMainCharacterAttributeSet* Attributes;

public:
	AUnrealSFASCharacter();

	void Kill(TEnumAsByte<EDeathCauses> DeathType);
	
		
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/* Called to return the GAS Component*/
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* Used to assign default values to the attributes*/
	virtual void InitializeAttributes();

	//Used to initialize the ability system on the server
	virtual void PossessedBy(AController* NewController) override; 


	/* Effect that initializes our default attributes*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	/* Effect for battery energy during time*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> TimeBatteryEnergyEffect;

	/* Effect for temperature by being near NPC*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> TemperatureEffectByAI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MaxTemperature = 40.f;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for front camera turn input */
	void TurnFrontCamera(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* Returns Axis value of moving forward input */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovingForwardAxisValue() const;

	UFUNCTION(BlueprintCallable, Category = "FlashLight")
	void ApplyFlashLight();

	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetTemperature() const;

	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetBatteryEnergy() const;

	void ChangeBatteryEnergy();

	void ChangeTemperatureByAI();

private:
	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetupStimulus();

	UPROPERTY()
	bool bDead = false;

	/* Character's death animation montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Montages", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticleEmitter;

	/* Character's AnimationInstance*/
	UPROPERTY()
	class UAnimInstance* CharacterAnimInstance;

	/* Axis value of moving forward input */
	UPROPERTY( )
	float MovingForwardAxisValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlashLight", meta = (AllowPrivateAccess = "true"))
	float FlashLightReach = 200;

};

