// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Animation/AnimInstance.h"
#include "RPGPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API URPGPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: 
    
	URPGPlayerAnimInstance(const FObjectInitializer& ObjectInitializer){};

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	class ARPGPlayerCharacter* Player;

	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	float Speed;

	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bIsInAir;
	
	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bDynamicFall;

    UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bIsAccelarating;
	
	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bCombat;

	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bGuard;

	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bDead;
    
	
};
