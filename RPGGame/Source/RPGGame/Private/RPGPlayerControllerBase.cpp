// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerControllerBase.h"
#include "Widgets/UIMainMenu.h"
#include "Widgets/UI_HUD.h"
#include "Blueprint/UserWidget.h"

void ARPGPlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();
}

void ARPGPlayerControllerBase::NotifyAllEnemiesDefeated()
{   
    bOnFight = false;
    OnAllEnemiesDefeated.Broadcast();
}

void ARPGPlayerControllerBase::NotifyOnEnemyFight()
{   
    bOnFight = true;
    OnEnemyFight.Broadcast();
}
