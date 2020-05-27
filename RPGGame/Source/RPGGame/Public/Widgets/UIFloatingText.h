// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "Blueprint/UserWidget.h"
#include "UIFloatingText.generated.h"

/**
 * 
 */
UCLASS()
class RPGGAME_API UUIFloatingText : public UUserWidget
{
	GENERATED_BODY()

public:
    
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable , Category = UI)
	void StartFloating(float Damage , AActor* Actor , bool IsDamage = true , bool RecoverHP = true);
	
};
