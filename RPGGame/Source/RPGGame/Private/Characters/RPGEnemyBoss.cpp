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

void ARPGEnemyBoss::BeginPlay()
{
    Super::BeginPlay();
}

void ARPGEnemyBoss::Destroyed()
{   
    if(bIsDead)
    {
        ARPGPlayerControllerBase* PlayerController = Cast<ARPGPlayerControllerBase>(UGameplayStatics::GetPlayerController(this , 0));
        if(PlayerController->GetClass()->ImplementsInterface(URPGUIInterface::StaticClass()))
        {
            IRPGUIInterface::Execute_BossIsDead(PlayerController,this);    
        }
    }
    

    
    
    Super::Destroyed();
}
