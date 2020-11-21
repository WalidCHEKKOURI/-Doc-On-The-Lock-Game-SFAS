// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BlackBoardKeys.h"
#include "UnrealSFASCharacter.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

ANPCAIController::ANPCAIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Obj(TEXT("BehaviorTree'/Game/AI/BT_NPC.BT_NPC'"));
	if (Obj.Succeeded())
	{
		BehaviorTree = Obj.Object;
	}
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SetupPerceptionSystem();
}


void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void ANPCAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
	
}

UBlackboardComponent* ANPCAIController::GetBlackboardComp() const
{
	return Blackboard;
}


void ANPCAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto const AICharacter = Cast<AUnrealSFASCharacter>(Actor))
	{

		GetBlackboardComp()->SetValueAsBool(bbKeys::can_see_player, Stimulus.WasSuccessfullySensed());
		UE_LOG(LogTemp, Warning, TEXT("WW_Sensed"));
	}
	UE_LOG(LogTemp, Warning, TEXT("WW_Target"));
}

void ANPCAIController::SetupPerceptionSystem()
{
	UE_LOG(LogTemp, Warning, TEXT("WW_Setup"));

	/* create and initialize sight configuration object */
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		/*add sight configuration component to perception component*/
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

}

