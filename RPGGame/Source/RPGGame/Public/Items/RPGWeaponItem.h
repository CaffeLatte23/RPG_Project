// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGItem.h"
#include "Items/RPGItem.h"
#include "RPGWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API URPGWeaponItem : public URPGItem
{
	GENERATED_BODY()

public:
   
    URPGWeaponItem()
	{
		ItemType = URPGAssetManager::WeaponItemType;
	}

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Weapon)
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Weapon")
	float BaseDamage = 20.f;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Weapon")
	float CriticalDamage = 10.f;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = "Weapon")
	float CriticalPercent = 0.5f;
	
};
