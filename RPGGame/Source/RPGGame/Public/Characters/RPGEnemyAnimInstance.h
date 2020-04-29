// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API URPGEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: 
    
	virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	class ARPGEnemyBase* Enemy;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	float Speed;
    
	UPROPERTY(BlueprintReadOnly , Category = Anim)
	float Direction;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	float Roll;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	float Yaw;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	float Pitch;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly , Category = Anim)
	bool bIsAccelerating = false;

	UPROPERTY(BlueprintReadOnly , Category = "CharAnim")
	bool bDamaged = false;
	
};
