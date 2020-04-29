// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

class URPGGameplayAbility;
/**
 * 
 */
UCLASS()
class RPGGAME_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    
	URPGAbilitySystemComponent();

	void GetActiveAbilitiesWithTags();
    
	UFUNCTION()
	int32 GetDefaultAbilityLevel() const;

	UFUNCTION()
	static URPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor , bool LookForComponent = false);
	
};
