// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RPGWeaponBase.h"
#include "Items/RPGWeaponItem.h"
#include "Components/CapsuleComponent.h"
#include "Characters/RPGCharacterBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARPGWeaponBase::ARPGWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	//メッシュの相対位置の変更のための仮設置
	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Root;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetupAttachment(RootComponent);
    
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this , &ARPGWeaponBase::OnOverlapBegin);
	WeaponCollision->OnComponentEndOverlap.AddDynamic(this , &ARPGWeaponBase::OnOverlapEnd);

	SetWeaponSpec();
}

// Called when the game starts or when spawned
void ARPGWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGWeaponBase::SetWeaponSpec()
{
	if(!Item)
	{   
		//生成元の親クラスがない場合、logに出力 , デフォルト値を設定
		UE_LOG(LogRPG , Warning , TEXT("This has No Parent Class"));
		BaseDamage = 20.f;
		CriticalDamage = 10.f;

		return;
	}

	BaseDamage = Item->BaseDamage;
	CriticalDamage = Item->CriticalDamage;
	
}

void ARPGWeaponBase::CollisionAble()
{   
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARPGWeaponBase::CollisionDisable()
{   
	DamagedActor.Reset();
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARPGWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{   
    //オーバーラップしたアクターが、オーナーではなく、オーナーが持つタグを所持していない（敵か味方か)
	ARPGCharacterBase* OtherChar = Cast<ARPGCharacterBase>(OtherActor);
	ARPGCharacterBase* WeaponOwner = Cast<ARPGCharacterBase>(this->GetOwner());
	
    if(OtherChar != this->GetOwner() && !OtherChar->ActorHasTag(OwnerTag) && OtherChar->IsAlive())
	{   
    //Damage処理
	   
		if(!(DamagedActor.Contains(OtherChar)))
		{
            if(OtherChar->GetClass()->ImplementsInterface(URPGCharacterInterface::StaticClass()))
		    {   
				UE_LOG(LogRPG , Warning , TEXT("Weapon :  Overlapped"));
                DamagedActor.Add(OtherChar);
				float Damage = WeaponOwner->CalclateDamage(BaseDamage , CriticalDamage);
				IRPGCharacterInterface::Execute_OnDamaged(OtherChar , Cast<ARPGCharacterBase>(this->GetOwner()) , Damage);
				
		    }
		}
	}
}

void ARPGWeaponBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{   

	if(OtherActor != this->GetOwner())
	{ 
		if(DamagedActor.Contains(OtherActor))
		{   
			DamagedActor.Remove(OtherActor);
		}
	}
}

