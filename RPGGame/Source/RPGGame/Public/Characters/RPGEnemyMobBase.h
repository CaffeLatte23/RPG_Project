// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Characters/RPGEnemyBase.h"
#include "RPGEnemyMobBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API ARPGEnemyMobBase : public ARPGEnemyBase
{
	GENERATED_BODY()

public: 
    ARPGEnemyMobBase();

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	//スポーンアニメーションの終了通知
	FOnSpawnFinished OnSpawnFinished;

	UPROPERTY(VisibleAnywhere , meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HPBar;

	UFUNCTION(BlueprintCallable , Category = Enemy)
	void UpdateBarVisibility(bool bIsHidden);
	
	//UFUNCTION()
	//virtual void OnDamaged_Implementation(ARPGCharacterBase* DamageCauser , float Damage) override;

///////////////////////////////////
/// drop

	UPROPERTY(EditDefaultsOnly , Category = "Drop")
	float PotionDropAmount = 0.05f;

	UPROPERTY(EditDefaultsOnly , Category = "Drop")
	float WeaponDropAmount = 0.1f;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly ,  Category = "Drop")
	TArray<TSubclassOf<class AItemPickUp>>PotionItemList;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly ,  Category = "Drop")
	TSubclassOf<class AItemPickUp> WeaponItemBase;

	UFUNCTION()
	void DropLoot();
	
};


