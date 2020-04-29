// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGEnemyAnimInstance.h"
#include "Characters/RPGEnemyBase.h"

void URPGEnemyAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
}

void URPGEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if(!TryGetPawnOwner())
    {
        return;
    }

    if(!Enemy)
    {
        Enemy = Cast<ARPGEnemyBase>(TryGetPawnOwner());
    }
    
    //速度
    Speed = Enemy->GetVelocity().Size();
    
    //方向
    Direction = CalculateDirection(Enemy->GetVelocity() , Enemy->GetActorRotation());

    //軸方向
    FRotator DeltaRotate = UKismetMathLibrary::NormalizedDeltaRotator( Enemy->GetBaseAimRotation() , Enemy->GetActorRotation());
    Roll = DeltaRotate.Roll;
    Yaw = DeltaRotate.Yaw;
    Pitch = DeltaRotate.Pitch;

    bIsInAir = Enemy->GetCharacterMovement()->IsFalling();

    //死亡判定
    bIsDead = Enemy->bIsDead;
    
    //加速判定
    bIsAccelerating = Enemy->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

    bDamaged = Enemy->bIsDamaged;


}

