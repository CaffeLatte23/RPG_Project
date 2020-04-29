// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Blueprint/UserWidget.h"
#include "Interface/RPGUIInterface.h"
#include "UI_HUD.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API UUI_HUD : public UUserWidget , public IRPGUIInterface
{
	GENERATED_BODY()

public:
    
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UVerticalBox* Statsgauge = nullptr;

	UFUNCTION(BlueprintImplementableEvent , Category = HUD)
	void EnemyOnFight();

	UFUNCTION(BlueprintImplementableEvent , Category = HUD)
	void AllDefeated();

	UFUNCTION()
	bool IsGaugeVisible();
	
};
