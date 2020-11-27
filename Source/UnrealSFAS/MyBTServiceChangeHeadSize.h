// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTServiceChangeHeadSize.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API UMyBTServiceChangeHeadSize : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UMyBTServiceChangeHeadSize();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	FString GetStaticServiceDescription() const override;
private:
		// NPC's head size
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float HeadSize = 1.0f;

	
};
