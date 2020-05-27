// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGEnemyMobBase.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/RPGStatusComponent.h"
#include "Weapon/RPGWeaponBase.h"
#include "Animation/AnimInstance.h"
#include "Items/ItemPickUp.h"
#include "Widgets/UIEnemyHP.h"

ARPGEnemyMobBase::ARPGEnemyMobBase()
{
    this->Tags.Add("Mob");

    HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusBar"));
    HPBar->SetupAttachment(this->GetMesh());
    HPBar->SetHiddenInGame(true);
    HPBar->SetRelativeLocation(FVector(0.f , 0.f , 200.f));
    HPBar->SetDrawSize(FVector2D(100.f , 16.f));
}

void ARPGEnemyMobBase::BeginPlay(){
    Super::BeginPlay();
    
    if(HPBar->GetUserWidgetObject())
    {
        HPWidget = HPBar->GetUserWidgetObject();
        UpdateHealth();
    }
}

void ARPGEnemyMobBase::Destroyed()
{   
    if(bIsDead)
    {
        DropLoot();
    }
    
    Super::Destroyed();
}


void ARPGEnemyMobBase::UpdateBarVisibility(bool bIsHidden)
{   
    if(HPBar->bHiddenInGame != !bIsHidden)
    {
        HPBar->SetHiddenInGame(!bIsHidden);
    }
   
}


void ARPGEnemyMobBase::DropLoot()
{

    if(UKismetMathLibrary::RandomBoolWithWeight(PotionDropAmount))
    {   
        if(PotionItemList.Num() -1 < 0)
        {
            return;
        }

        //Spawn Potion
        TSubclassOf<AItemPickUp> PickUpClass = PotionItemList[UKismetMathLibrary::RandomInteger(PotionItemList.Num())];
        FTransform SpawnLocation = FTransform( GetActorLocation() + FVector( 0.f , 0.f , 30.f));
            
        GetWorld()->SpawnActor<AItemPickUp>(PickUpClass , SpawnLocation);   
    }
    else if(UKismetMathLibrary::RandomBoolWithWeight(WeaponDropAmount))
    {
        if(WeaponItemBase != nullptr)
        {
            FTransform SpawnLocation = FTransform( GetActorLocation() + FVector( 0.f , 0.f , 30.f));

            AItemPickUp* WeaponDrop =  GetWorld()->SpawnActorDeferred<AItemPickUp>(WeaponItemBase , SpawnLocation , this);
            WeaponDrop->ItemType = WeaponItem;
            WeaponDrop->FinishSpawning(SpawnLocation);
        }
    }
        
}

