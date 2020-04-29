// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/RPGAbilitySystemComponent.h"
#include "Characters/RPGCharacterBase.h"
#include "AbilitySystemGlobals.h"

URPGAbilitySystemComponent::URPGAbilitySystemComponent(){}

int32 URPGAbilitySystemComponent::GetDefaultAbilityLevel() const
{
    ARPGCharacterBase* OwningCharacter = Cast<ARPGCharacterBase>(OwnerActor);

    if(OwningCharacter)
    {
        return OwningCharacter->CharStatus.CharLevel;
    }

    return 1;
}

URPGAbilitySystemComponent* URPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor , bool LookForComponent)
{
    return Cast<URPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor , LookForComponent));
}