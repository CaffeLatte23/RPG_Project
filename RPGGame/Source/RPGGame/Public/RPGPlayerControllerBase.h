// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "GameFramework/PlayerController.h"
#include "Interface/RPGUIInterface.h"
#include "RPGPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API ARPGPlayerControllerBase : public APlayerController , public IRPGUIInterface 
{
	GENERATED_BODY()

public: 
    
	ARPGPlayerControllerBase() {}
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite , Category = "UI")
	bool bOpenUI = false;

	UPROPERTY(BlueprintReadWrite , Category = "UI")
	bool bAnyChange = false;

	UPROPERTY(BlueprintReadWrite , Category = UI)
	class UUI_HUD* HUD;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Fight")
	bool bOnFight = false;

	//敵の全滅通知
	FOnAllEnemiesDefeated OnAllEnemiesDefeated;
    
	//戦闘態勢
	FOnEnemyFight OnEnemyFight;

	UFUNCTION(BlueprintImplementableEvent , Category = UI)
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent , Category = UI)
	void UpdateHP(float Value);

	UFUNCTION(BlueprintImplementableEvent , Category = UI)
	void UpdateIcons();

	UFUNCTION(BlueprintImplementableEvent , Category = UI)
	void TargetIsBoss(class ARPGEnemyBase* Actor);

	UFUNCTION(BlueprintImplementableEvent , Category = UI)
	void BossIsDead(AActor* Actor);


	UFUNCTION(BlueprintCallable , Category = Fight)
	void NotifyAllEnemiesDefeated();

	UFUNCTION(BlueprintCallable , Category = Fight)
	void NotifyOnEnemyFight();


};
