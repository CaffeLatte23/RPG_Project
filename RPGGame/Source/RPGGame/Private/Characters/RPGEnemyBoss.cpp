// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGEnemyBoss.h"
#include "RPGPlayerControllerBase.h"
#include "Dungeon/EnemySpawnerVolume.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGStatusComponent.h"

ARPGEnemyBoss::ARPGEnemyBoss()
{
    this->Tags.Add("Boss");
    
    
}

void ARPGEnemyBoss::Destroyed()
{
    ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0));

    if(PlayerController->GetClass()->ImplementsInterface(URPGUIInterface::StaticClass()))
    {
        IRPGUIInterface::Execute_BossIsDead(PlayerController,this);    
    }
    
    Super::Destroyed();
}

void ARPGEnemyBoss::OnDamaged_Implementation(ARPGCharacterBase* DamageCauser  , float Damage)
{
    if(!bIsGuard)
    {
        CharStatus.HP -= Damage;
    }

    ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0));
    if(PlayerController && PlayerController->GetClass()->ImplementsInterface(URPGUIInterface::StaticClass()))
    {
        IRPGUIInterface::Execute_UpdateHP(PlayerController , CharStatus.HP / StatusComp->OwnerStatus.HP);
    }

    if(FMath::Clamp(CharStatus.HP , 0.f , StatusComp->OwnerStatus.HP) <= 0.f && !bIsDead)
    {
        bIsDead = true;

        UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();

        if(AnimInstance->Montage_IsPlaying(AnimInstance->GetCurrentActiveMontage()))
        {
            StopAnimMontage(AnimInstance->GetCurrentActiveMontage());
        }


        if(ParentVolume)
        {
            ParentVolume->DefeatedActor(this);
        }

        if(this->GetController())
        {
            this->GetController()->UnPossess();
        }

        GetWorldTimerManager().SetTimer(Handle , [this](){
            this->Destroy();
        },2.f , false);
    }
    else
    {
        bIsDamaged = true;

        if(HitReactMotion)
        {
            PlayAnimMontage(HitReactMotion);
            GetWorldTimerManager().SetTimer(Handle , [this](){
               bIsDamaged = false;
            }, 1.0f , false);
        }
    }
}
