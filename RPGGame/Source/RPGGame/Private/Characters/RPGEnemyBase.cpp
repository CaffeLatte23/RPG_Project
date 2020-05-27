// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGEnemyBase.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/RPGWeaponItem.h"
#include "Weapon/RPGWeaponBase.h"
#include "Interface/RPGUIInterface.h"
#include "Curves/CurveVector.h"
#include "Animation/AnimInstance.h"

ARPGEnemyBase::ARPGEnemyBase()
{   
    this->bUseControllerRotationYaw = true;
    this->bUseControllerRotationRoll = false;
    this->bUseControllerRotationPitch = false;

    this->Tags.Add("Enemy");
    LockWC = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOn"));
    LockWC->SetupAttachment(this->GetMesh() , "TargetPoint");
    LockWC->SetDrawSize(FVector2D(32.f,32.f));
    LockWC->SetHiddenInGame(true);
    
    ///右側　コリジョン
    Collision_R = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collsion_R"));
    Collision_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Collision_R->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera , ECollisionResponse::ECR_Ignore);
    Collision_R->SetupAttachment(this->GetMesh() , "Weapon_Right");
    Collision_R->OnComponentBeginOverlap.AddDynamic(this , &ARPGEnemyBase::OnOverlapBegin);
    Collision_R->OnComponentEndOverlap.AddDynamic(this , &ARPGEnemyBase::OnOverlapEnd);
    
    ///左側　コリジョン
    Collision_L = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collsion_L"));
    Collision_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Collision_L->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera , ECollisionResponse::ECR_Ignore);
    Collision_L->SetupAttachment(this->GetMesh() , "Weapon_Left");
    Collision_L->OnComponentBeginOverlap.AddDynamic(this , &ARPGEnemyBase::OnOverlapBegin);
    Collision_L->OnComponentEndOverlap.AddDynamic(this , &ARPGEnemyBase::OnOverlapEnd);
    
    //DestoryEffect
    const ConstructorHelpers::FObjectFinder<UParticleSystem> FX_Destory(TEXT("ParticleSystem'/Game/RPGGame/Effects/FX_Minion/FX/Particles/Minions/Minion_melee/FX/Death/P_MeleeMinion_Chunks_Ranged.P_MeleeMinion_Chunks_Ranged'"));
    if(FX_Destory.Object)
    {
        DestoryEffect = FX_Destory.Object;
    }
    
    //HitEffect
    const ConstructorHelpers::FObjectFinder<UParticleSystem> FX_Hit(TEXT("ParticleSystem'/Game/RPGGame/Effects/FX_Particle/P_PP.P_PP'"));
    if(FX_Hit.Object)
    {
        HitEffect = FX_Hit.Object;
    }
    
    //HitSound
    const ConstructorHelpers::FObjectFinder<USoundBase> SFX_Hit(TEXT("SoundCue'/Game/RPGGame/Assets/Sounds/Weapons/Weapon_Hit.Weapon_Hit'"));
    if(SFX_Hit.Object)
    {
        HitSound = SFX_Hit.Object;
    }


    // タイムライン
    Timeline = new FTimeline();
    Timeline->SetTimelineLength(0.8f);

    const ConstructorHelpers::FObjectFinder<UCurveVector> Interp(TEXT("CurveVector'/Game/RPGGame/DataAsset/Curve/SpawnSizeCurve.SpawnSizeCurve'"));
    FOnTimelineVector ScaleValue;
    ScaleValue.BindUFunction(this , "UpdateRelativeSize");
    Timeline->AddInterpVector(Interp.Object , ScaleValue);

    ///Timeline Finished
    FOnTimelineEvent OnTimelineFinished;
    OnTimelineFinished.BindUFunction(this , "OnSpawnFinish");
    Timeline->SetTimelineFinishedFunc(OnTimelineFinished);
}

void ARPGEnemyBase::BeginPlay()
{
    Super::BeginPlay();
    Player = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this , 0));

    if(WeaponItemArray.IsValidIndex(0))
    {   
        Collision_L->DestroyComponent();
        Collision_R->DestroyComponent();
        SpawnWeapon();
    }
    else
    {
        CollisionMap.Add(EEnableCollisionPoint::RightHand , Collision_R);
        CollisionMap.Add(EEnableCollisionPoint::LeftHand  , Collision_L);
    }

    if(bEnableSpawnEffect)
    {
        OnSpawning();
    }
}

void ARPGEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if(Timeline != nullptr && Timeline->IsPlaying())
    {
        Timeline->TickTimeline(DeltaTime);
    }

}

void ARPGEnemyBase::OnSpawning()
{  
    if(Timeline)
    {   
        FTransform Transform = GetActorTransform();
        Transform.SetScale3D(FVector(FinalScale));
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , SpawnEffect , Transform);
        Timeline->PlayFromStart();
    }
    
}

void ARPGEnemyBase::OnSpawnFinish()
{   
    OnSpawnFinished.ExecuteIfBound();
}

void ARPGEnemyBase::Destroyed()
{
    FTransform Transform = GetActorTransform();
    float CapsulehalfHeight = this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    Transform.SetLocation(FVector(Transform.GetLocation().X , Transform.GetLocation().Y ,CapsulehalfHeight));
    Transform.SetScale3D(FVector(FinalScale));
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , DestoryEffect , Transform);
}

void ARPGEnemyBase::UpdateRelativeSize(FVector Value)
{
    this->GetMesh()->SetRelativeScale3D(Value * FinalScale);
}

void ARPGEnemyBase::SpawnWeapon()
{   
    FActorSpawnParameters Param;
    Param.Owner = this;
    Param.Instigator = this;

    int32 Idx = FMath::RandRange(0 , WeaponItemArray.Num() - 1);
    WeaponItem = WeaponItemArray[Idx];

    URPGWeaponItem* WeaponClass = Cast<URPGWeaponItem>(WeaponItem);
    if(WeaponClass)
    {
        CurrentWeapon = GetWorld()->SpawnActor<ARPGWeaponBase>(WeaponClass->WeaponActor , GetActorLocation(), GetActorRotation() , Param);
        CurrentWeapon->OwnerTag = "Enemy";
        CurrentWeapon->AttachToComponent(this->GetMesh() , FAttachmentTransformRules::SnapToTargetIncludingScale , "GSEquip_R" );
        CurrentWeapon->BaseDamage = BaseDamage;
        CollisionMap.Add(EEnableCollisionPoint::Weapon ,CurrentWeapon->Root);
    }
    
}

void ARPGEnemyBase::UpdateCombatType(ECombatType NewType)
{   
    if(CombatType == ECombatType::Attack)
    {
        ParentVolume->AttackTaskFinished();
    }

    CombatType = NewType;
}

void ARPGEnemyBase::UpdateHealth()
{
    //HPの更新
    if(HPWidget && HPWidget->GetClass()->ImplementsInterface(URPGUIInterface::StaticClass()))
    {   
        IRPGUIInterface::Execute_UpdateHP(HPWidget , CharStatus.HP / ParentStatus.HP);
    }
}

void ARPGEnemyBase::Attack()
{
    if(!CurrentAttackMontage.IsValidIndex(CurrentCombo))
    {   return; }
        
    UAnimMontage* ComboMontage = CurrentAttackMontage[CurrentCombo];
    bIsAttack = true;
    PlayAnimMontage(ComboMontage , ComboPlayRate);
    CurrentCombo++;
}

void ARPGEnemyBase::EnableCollision(EEnableCollisionPoint CollisionPoint)
{   
    if(CollisionPoint == EEnableCollisionPoint::Weapon)
    {
        CurrentWeapon->CollisionAble();
    }
    else
    {
        UCapsuleComponent* Collision = *CollisionMap.Find(CollisionPoint);
        if(Collision)
        {
            Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        } 
    }
    
}

void ARPGEnemyBase::DisableCollision(EEnableCollisionPoint CollisionPoint)
{

    if(CollisionPoint == EEnableCollisionPoint::Weapon)
    {
        CurrentWeapon->CollisionDisable();
    }
    else
    {
        UCapsuleComponent* Collision = *CollisionMap.Find(CollisionPoint);
        if(Collision)
        {
            Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        } 
    }
}

void ARPGEnemyBase::ApplyDamage(AActor* OtherActor)
{   
    if(OtherActor->ActorHasTag("Player") )
    {
        IRPGCharacterInterface::Execute_OnDamaged(OtherActor , this , BaseDamage + (int32)(FMath::RandRange(0.f , CharStatus.Attack / 2.0f)));
    }
}

void ARPGEnemyBase::HitStopHandle()
{   
    FVector BackVector = GetActorForwardVector() * (-1.f) * (600.f); 
    LaunchCharacter(BackVector , true ,true);

    float SpawnHeight = this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    UGameplayStatics::SpawnEmitterAttached(HitEffect , this->GetMesh() , "TargetPoint");
    UGameplayStatics::SpawnSoundAttached(HitSound , this->GetMesh() , "TargetPoint" , FVector() , FRotator() , EAttachLocation::KeepRelativeOffset , false , 0.3f);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld() , 0.01);
    
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle , [this](){
        UGameplayStatics::SetGlobalTimeDilation(GetWorld() , 1);
    } , 0.0004f , false);
}

void ARPGEnemyBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{   
    if(!DamagedActors.Contains(OtherActor) )
    {
        ApplyDamage(OtherActor);
    }
}

void ARPGEnemyBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    DamagedActors.Reset();
}

void ARPGEnemyBase::NotifyTarget_Implementation(bool bTarget)
{
    LockWC->SetHiddenInGame(!bTarget);
}

void ARPGEnemyBase::OnDamaged_Implementation(ARPGCharacterBase* DamageCauser  , float Damage)
{
    //共通部分の処理
    if(bIsDead)
    {
        return;
    }
    CharStatus.HP -= Damage;
    FloatDamageText(Damage,this);
    HitStopHandle();

    UpdateHealth();

    if(CharStatus.HP > 0.f)
    {
        DamageAction();
    }
    else
    {
        DeadAction();
    }
    
}

void ARPGEnemyBase::DamageAction()
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

void ARPGEnemyBase::DeadAction()
{
    bIsDead = true;
    
    //モンタージュが再生されていれば、停止させる
    UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
    if(AnimInstance->IsAnyMontagePlaying())
    {
        StopAnimMontage(AnimInstance->GetCurrentActiveMontage());
    }
    
    //他への死亡通知
    IRPGCharacterInterface::Execute_DefeatEnemy(Player , this);
    if(ParentVolume)
    {
        ParentVolume->DefeatedActor(this);
    }
    
    //動かないようにコントローラーを排除
    if(this->GetController())
    {   
        this->GetController()->UnPossess();
    }

    //自分の破壊       
    GetWorldTimerManager().SetTimer(Handle , [this](){
    if(CurrentWeapon)
    {
        CurrentWeapon->Destroy();
    } 
    this->Destroy();
    } , 3.0f , false);
}




