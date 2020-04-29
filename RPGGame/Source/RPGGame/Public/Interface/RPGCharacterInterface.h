// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "UObject/Interface.h"
#include "RPGCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPGCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGGAME_API IRPGCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    
	//ターゲットされたアクターへの通知
	UFUNCTION(BlueprintCallable , BlueprintNativeEvent , Category = "Interface")
	void NotifyTarget(bool bTarget);

	//ダメージ通知
	UFUNCTION(BlueprintCallable , BlueprintNativeEvent , Category = "Interface")
	void OnDamaged(class ARPGCharacterBase* DamageCauser , float Damage);

	//
	UFUNCTION(BlueprintCallable , BlueprintNativeEvent , Category = "Interface")
	void DefeatEnemy(ARPGEnemyBase* Enemy);
};
