// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGEnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API ARPGEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
    ARPGEnemyAIControllerBase();

    virtual void OnPossess(class APawn* InPawn) override;
    
	UFUNCTION()
	void BehaiviorSetup();

	UPROPERTY()
	class ARPGEnemyBase* PosCharacter;

	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly ,  meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BBComp;
   
    UPROPERTY(EditDefaultsOnly , BlueprintReadOnly ,  Category = AI)
	class UBlackboardData* BBData;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = AI)
	class UBehaviorTree* BTData;

	UPROPERTY(EditDefaultsOnly , Category = AI)
	float ArrowDistance = 150.f;

	UPROPERTY(EditDefaultsOnly , Category = AI)
	float AttackArrowLine = 0.5f;

	UPROPERTY(EditDefaultsOnly , Category = AI)
	float InnerRadius = 500.f;

	UPROPERTY(EditDefaultsOnly , Category = AI)
	float OuterRadius = 900.f;

	UPROPERTY(EditDefaultsOnly , Category = AI)
	int32 RingNum = 3;
	
};
