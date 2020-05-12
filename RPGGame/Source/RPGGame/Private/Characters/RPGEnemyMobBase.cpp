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
    UpdateHealth();
}

void ARPGEnemyMobBase::Destroyed()
{   
    DropLoot();
    Super::Destroyed();
}

void ARPGEnemyMobBase::UpdateHealth()
{   
    WB_HP = Cast<UUIEnemyHP>(HPBar->GetUserWidgetObject());
    if(WB_HP)
    {
        WB_HP->SetPercent(CharStatus.HP / StatusComp->OwnerStatus.HP);
    }
}

void ARPGEnemyMobBase::UpdateBarVisibility(bool bIsHidden)
{   
    if(HPBar->bHiddenInGame != !bIsHidden)
    {
        HPBar->SetHiddenInGame(!bIsHidden);
    }
   
}

void ARPGEnemyMobBase::OnDamaged_Implementation(ARPGCharacterBase* DamageCauser , float Damage)
{   
    
    CharStatus.HP  -= Damage;
   
    UpdateHealth();
    
    FVector LaunchVelocity = GetActorForwardVector() * -1.f * 1000.f; 
    this->LaunchCharacter(LaunchVelocity , true , true);
    FTimerHandle HitHandle;
    GetWorldTimerManager().SetTimer( HitHandle , [this](){
        HitStopHandle();
    } , 0.05f , false);
    

    if(CharStatus.HP <= 0.f)
    {   
        if(!GetWorldTimerManager().IsTimerPaused(Handle))
        {
            GetWorldTimerManager().PauseTimer(Handle);
        }

        UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();

        if(AnimInstance->Montage_IsPlaying(AnimInstance->GetCurrentActiveMontage()))
        {
            StopAnimMontage(AnimInstance->GetCurrentActiveMontage());
        }

        bIsDead = true;
        if(ParentVolume)
        {
            ParentVolume->DefeatedActor();
        }


        if(Player->GetClass()->ImplementsInterface(URPGCharacterInterface::StaticClass()))
        {   
            
            if(this->GetController())
            {   
                IRPGCharacterInterface::Execute_DefeatEnemy(Player , this);
                this->GetController()->UnPossess();
            }
            
            GetWorldTimerManager().SetTimer(Handle , [this](){
                if(CurrentWeapon)
                {
                    CurrentWeapon->Destroy();
                } 
                this->Destroy();
            } , 3.0f , false);

        }

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

