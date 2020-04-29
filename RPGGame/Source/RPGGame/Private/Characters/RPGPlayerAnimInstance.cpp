// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGPlayerAnimInstance.h"
#include "Characters/RPGPlayerCharacter.h"

void URPGPlayerAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
}

void URPGPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    if(!TryGetPawnOwner())
    {
        return;
    }

    if(!Player)
    {
        Player = Cast<ARPGPlayerCharacter>(TryGetPawnOwner());
    }
    
    //移動スピード
    Speed = Player->GetVelocity().Size();
    
    //空中にいるか
    bIsInAir = Player->GetMovementComponent()->IsFalling();
    
    //受け身モーションのフラグ
    bDynamicFall = Player->bDynamicFall;

    bIsAccelarating = (Player->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 );

    bCombat = Player->bIsCombat;

    bGuard = Player->bIsGuard;

    bDead = Player->bIsDead;
}


