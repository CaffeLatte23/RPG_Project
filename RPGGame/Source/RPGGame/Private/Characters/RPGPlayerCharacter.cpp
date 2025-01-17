// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGPlayerCharacter.h"
#include "Characters/RPGEnemyBase.h"
#include "Characters/RPGEnemyMobBase.h"
#include "Weapon/RPGWeaponBase.h"
#include "Animation/AnimInstance.h"
#include "RPGPlayerControllerBase.h"
#include "Widgets/UI_HUD.h"
#include "Components/RPGInventoryComponent.h"
#include "Components/RPGStatusComponent.h"
#include "Items/RPGWeaponItem.h"
#include "Items/RPGPotionItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPGPlayerControllerBase.h"

ARPGPlayerCharacter::ARPGPlayerCharacter()
{   
    this->Tags.Add("Player");

    // SpringArm
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.f;
    CameraBoom->SocketOffset = FVector(0.f , 0.f , 130.f);
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bDoCollisionTest = false;

    // CameraActor
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom , USpringArmComponent::SocketName);
    FollowCamera->SetRelativeRotation(FRotator(-10.f,0.f,0.f));
    FollowCamera->bUsePawnControlRotation = false;

    //PlayerSight
    PlayerSight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerSight"));
    PlayerSight->SetupAttachment(FollowCamera);

    //InventoryComponent
    InventoryComp = CreateDefaultSubobject<URPGInventoryComponent>(TEXT("Inventory Component"));
    
    
    //TimeLine
    Timeline = new FTimeline();
    Timeline->SetTimelineLength(0.5f);
    
    //Curve finder
    const ConstructorHelpers::FObjectFinder<UCurveFloat> InterpAccelarate(TEXT("CurveFloat'/Game/RPGGame/DataAsset/Curve/Curve_Accelarate.Curve_Accelarate'"));
    FOnTimelineFloat AccelarateValue;
    AccelarateValue.BindUFunction(this , "UpdateMoveSpeed");
    Timeline->AddInterpFloat(InterpAccelarate.Object , AccelarateValue);

}

void ARPGPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<ARPGPlayerControllerBase>(this->GetController());
    if(PlayerController)
    { 
        PlayerController->OnEnemyFight.AddLambda([this](){
            if(bIsCombat != true)
            {
                EquipAction();
            }
        });

        PlayerController->OnAllEnemiesDefeated.AddLambda([this](){
            if(bIsCombat != false)
            {
                EquipAction();
            }
        });
    }

    GetWorldTimerManager().SetTimer(SearchHandle , [this](){
        PreTarget = CheckEnemy();
    } , 0.05f , true, 0.3f);
}

void ARPGPlayerCharacter::Tick(float DeltaTime )
{
    Super::Tick(DeltaTime);
    
    if(bCameraUpdate)
    {
        CameraMove();
    }

    if(Timeline != nullptr && Timeline->IsPlaying())
    {
        Timeline->TickTimeline(DeltaTime);
    }

}

void ARPGPlayerCharacter::RecoveryMPStart()
{
    if(GetWorldTimerManager().IsTimerPaused(SprintTimeHandle))
    {
        GetWorldTimerManager().UnPauseTimer(SprintTimeHandle);
    }
    else
    {
        GetWorldTimerManager().SetTimer(SprintTimeHandle , [this](){
            RecoveryMP();
        } , 0.01f , true);
    }
    
    
}

void ARPGPlayerCharacter::RecoveryMP()
{
    if(CharStatus.MP <= ParentStatus.MP )
    {
        CharStatus.MP += 0.1f;
    }
    else
    {
        GetWorldTimerManager().ClearTimer(SprintTimeHandle);
    }
}


//////////////////////////////////////////////////////////////////////////
// Camera

void ARPGPlayerCharacter::CameraMove()
{
    UpdateCameraType(bLockedOnTarget);
    
    if(bLockedOnTarget)
    {
        bool bInput = (GetInputAxisValue("Turn") == 0.f) && (GetInputAxisValue("LookUp") == 0.f);

        float InterpSpeed = (bInput) ? 0.8f : 0.2f;

        //Target Camera
        if(TargetLockActor != nullptr)
        {   
            FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation() , TargetLockActor->GetActorLocation());
            FRotator ControlRot = FMath::RInterpTo(GetControlRotation() , TargetRot , GetWorld()->GetDeltaSeconds() , InterpSpeed);
            Controller->SetControlRotation(FRotator(FMath::Clamp( ControlRot.Pitch , -20.f , 20.f), ControlRot.Yaw , 0.f));
        }
        

    }
    else
    {
        if(GetVelocity().Size() <= 0.f)
        {
            return;
        }

        FVector RootVec =  UKismetMathLibrary::GetForwardVector( FRotator(0.f , RootComponent->GetComponentRotation().Yaw , 0.f) );
        FVector CameraVec = UKismetMathLibrary::GetForwardVector( FRotator(0.f , FollowCamera->GetComponentRotation().Yaw , 0.f));
        float DotVec = FVector::DotProduct(RootVec , CameraVec);
        

        if(DotVec < -0.6f && DotVec > -0.9f)
        {
            CameraBoom->CameraRotationLagSpeed = 0.5f;
            FRotator TargetRot = FMath::RInterpTo(GetControlRotation() ,RootComponent->GetComponentRotation() , GetWorld()->GetDeltaSeconds() , 0.5f);

            Controller->SetControlRotation(TargetRot);
        }
    }
    
}

void ARPGPlayerCharacter::UpdateCameraType(bool bTarget)
{ 
    if(!bIsDead)
    {
        float FOV = (bIsCombat) ? 95.f : 90.f;
        float ArmLen = (bIsCombat) ? 420.f : 400.f;
        FRotator CameraRot = (bTarget) ? FRotator(-15.f ,0.f,0.f) : FRotator( -10.f ,0.f , 0.f);
        FVector TargetPos = (bTarget) ? FVector(0.f,0.f,30.f) : FVector(0.f,0.f,0.f);
    
        CameraBoom->TargetOffset = FMath::VInterpTo(CameraBoom->TargetOffset,TargetPos , GetWorld()->GetDeltaSeconds() , 1.0f);
        CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength , ArmLen , GetWorld()->GetDeltaSeconds() , 1.0f);
        FollowCamera->SetRelativeRotation(FMath::RInterpTo(FollowCamera->GetRelativeRotation() , CameraRot , GetWorld()->GetDeltaSeconds() , 1.0f));
        FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView , FOV , GetWorld()->GetDeltaSeconds() , 1.0f));
    }
    else
    {
        float ArmLen = 500.f;
        CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength , ArmLen , GetWorld()->GetDeltaSeconds() , 3.0f);
    }
    
    
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    //Axis Input
    PlayerInputComponent->BindAxis("MoveForward" , this ,&ARPGPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight" , this , &ARPGPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn" , this , &ARPGPlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp" , this , &ARPGPlayerCharacter::LookAtRate);

    //Action Input
    PlayerInputComponent->BindAction("Jump" , IE_Pressed , this ,&ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump" , IE_Released , this , &ARPGPlayerCharacter::StopJumping);
    PlayerInputComponent->BindAction("Dash" , IE_Pressed , this , &ARPGPlayerCharacter::DashAction);
    PlayerInputComponent->BindAction("Dash" , IE_Released , this , &ARPGPlayerCharacter::EndSprint);
    PlayerInputComponent->BindAction("Attack" , IE_Pressed , this ,&ARPGPlayerCharacter::MeleeAttack);
    PlayerInputComponent->BindAction("TargetLock" , IE_Pressed , this , &ARPGPlayerCharacter::TargetLockEvent);
    PlayerInputComponent->BindAction("SwitchTarget" , IE_Pressed , this , &ARPGPlayerCharacter::SwitchTarget);
    PlayerInputComponent->BindAction("Guard" , IE_Pressed , this , &ARPGPlayerCharacter::GuardAction);
    PlayerInputComponent->BindAction("Guard" , IE_Released , this , &ARPGPlayerCharacter::GuardAction);
    PlayerInputComponent->BindAction("ChargeAttack" , IE_Pressed , this , &ARPGPlayerCharacter::ChargeAttackStart);
    PlayerInputComponent->BindAction("ChargeAttack" , IE_Released , this , &ARPGPlayerCharacter::ChargeAttackEnd);
    PlayerInputComponent->BindAction("ItemUse1" , IE_Pressed , this , &ARPGPlayerCharacter::Slot1ItemUse);
    PlayerInputComponent->BindAction("ItemUse2" , IE_Pressed , this , &ARPGPlayerCharacter::Slot2ItemUse);

}

EDirectionType ARPGPlayerCharacter::GetInputDirection()
{
    
    //軸の優先を決める
    if(UKismetMathLibrary::Abs(GetInputAxisValue("MoveForward")) >= UKismetMathLibrary::Abs(GetInputAxisValue("MoveRight")))
    {   
        //Y軸判定
        if(GetInputAxisValue("MoveForward") > 0)
        {
            return EDirectionType::Forward;
        }

        return EDirectionType::Behind;
    }
    else
    {
        //X軸判定
        if(GetInputAxisValue("MoveRight") > 0)
        {
            return EDirectionType::Right;
        }

        return EDirectionType::Left;
    }
    
    return EDirectionType::Forward;
}

//////////////////////////////////////////////////////////////////////////
// Combat

void ARPGPlayerCharacter::MoveForward(float Value)
{
    if((Controller != NULL) && (Value != 0.f) && !CombatCheck())
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f , Rotation.Yaw , 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction , Value);
    }
}

void ARPGPlayerCharacter::MoveRight(float Value)
{
    if((Controller != NULL) && (Value != 0.f) && !CombatCheck())
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f , Rotation.Yaw , 0.f);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction , Value);
    }
}

void ARPGPlayerCharacter::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerCharacter::LookAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ARPGPlayerCharacter::CombatCheck()
{
    return bIsGuard || bIsAttack || bMoveCheck;
}

void ARPGPlayerCharacter::DashAction()
{  
    if(bIsCombat)
    {
        //ローリングモーション中にイベントが起きないように
        if(bIsDash)
        {   
            return; 
        }
        //Rolling アクション
        bIsDash = true;
        
        if(GetWorldTimerManager().IsTimerActive(SprintTimeHandle))
        {
            GetWorldTimerManager().PauseTimer(SprintTimeHandle);
        }


        CharStatus.MP -= 10;
        PlayAnimMontage(RollingMontage , 1.0f , "Default");
        

        FTimerHandle Handle;
        GetWorldTimerManager().SetTimer(Handle , [this](){
            bIsDash = false;
            ResetComboValue();
            RecoveryMPStart();
        } , 0.5f ,false);
    }
    else
    {   
       
       //納刀時はSprintAction 
       bIsDash = true;
       if(GetWorldTimerManager().IsTimerActive(SprintTimeHandle))
       {
            GetWorldTimerManager().PauseTimer(SprintTimeHandle);
       }

       if(GetCharacterMovement()->MaxWalkSpeed <= BaseSpeed)
       {   
            Timeline->Play();
       }

       if(PlayerController->bOnFight)
       {   
           GetWorldTimerManager().SetTimer(SprintTimeHandle , [this](){
               
               if(CharStatus.MP > 0 )
               {
                   CharStatus.MP -= 0.1f;
               }
               else
               {
                    EndSprint();    
               }
           } , 0.01f , true);
       }
       
    }
   
}

void ARPGPlayerCharacter::EndSprint()
{   
    if(bIsCombat)
    {
        return;
    }

    bIsDash = false;
    GetWorldTimerManager().ClearTimer(SprintTimeHandle);
    RecoveryMPStart();
    Timeline->Reverse();
}

void ARPGPlayerCharacter::UpdateMoveSpeed(float Value)
{   
    
    GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + Value * (MaxSpeed - BaseSpeed);
}

void ARPGPlayerCharacter::StopJumping()
{
    ACharacter::StopJumping();
    FTimerHandle Handle;
    
    //DebugLine
    FVector Start = GetActorLocation();
    FVector End =  GetActorLocation() + GetActorUpVector()* -10000000.f;
    FCollisionQueryParams params;

    
    //着地モーション用の高さ判定
    GetWorldTimerManager().SetTimer(Handle , [=](){
        FHitResult OutHit;

        GetWorld()->LineTraceSingleByChannel(
           OutHit,
           Start,
           End,
           ECollisionChannel::ECC_Visibility,
           params
        );

        if(OutHit.Distance > 750.f)
        {
            bDynamicFall = true;
        }

    } , 1.5f , false);
}

void ARPGPlayerCharacter::GuardAction()
{
    if(!bIsCombat)
    {
        return;
    }
    
    FTimerHandle Handle;

    //Press
    if(!bIsGuard)
    {
        bIsGuard = true;
        bMoveCheck = true;
        bEnableJustGuard = true;

        GetWorldTimerManager().SetTimer(Handle , [this](){
            bEnableJustGuard = false;
        } , 0.5f , false);
    }
    else // Release
    {
        bIsGuard = false;

        GetWorldTimerManager().SetTimer(Handle , [this](){
            bMoveCheck = false;
        } , 0.25f , false);

    }

}

void ARPGPlayerCharacter::Slot1ItemUse()
{
    if(InventoryComp->GetSlottedItem(FRPGItemSlot("Potion" , 0)))
    {
        UseItem(0);
    }
}

void ARPGPlayerCharacter::Slot2ItemUse()
{
    if(InventoryComp->GetSlottedItem(FRPGItemSlot("Potion" , 1)))
    {
        UseItem(1);
    }
}

void ARPGPlayerCharacter::EquipAction()
{
    if(EquipMontage)
    {
        FName SectionName = bIsCombat ? "UnEquip" : "Equip";

        PlayAnimMontage(EquipMontage , 1.0f , SectionName);
    }
}

void ARPGPlayerCharacter::CreateAllWeapons()
{
    for(ARPGWeaponBase* Weapon : EquippedWeapons)
    {
        Weapon->Destroy();
    }
    EquippedWeapons.Reset();

    InventoryComp->GetSlottedItems(EquippedRPGItems , "Weapon" , false);

    for(int32 idx = 0 ; idx < EquippedRPGItems.Num() ; idx++)
    {
        URPGWeaponItem* Item = Cast<URPGWeaponItem>(EquippedRPGItems[idx]);
        
        FTransform SpawnTransform = FTransform(FRotator(0.f,0.f,0.f) , FVector(0.f,0.f,-1000.f) , FVector(1.f,1.f,1.f));
        
        ARPGWeaponBase* Weapon = GetWorld()->SpawnActorDeferred<ARPGWeaponBase>(Item->WeaponActor , SpawnTransform ,this );
        Weapon->Item = Item;
        Weapon->Slot = FRPGItemSlot("Weapon" , idx);
        Weapon->OwnerTag = "Player";
        Weapon->FinishSpawning(SpawnTransform);

        EquippedWeapons.Add(Weapon);
    }

    if(bFirstAttach)
    {   
        AttachNextWeapon();
        bFirstAttach = false;
    }
    else
    {   
        if(EquippedWeapons.IsValidIndex(CurrentWeaponIndex + 1))
        {   
           
            WeaponAttachMethod(CurrentWeaponIndex);
        }
        else
        {   
            AttachNextWeapon();
        }
    }
    
}

int32 ARPGPlayerCharacter::AttachNextWeapon()
{
    CurrentWeaponIndex = 0;

    if(CurrentWeaponIndex > EquippedWeapons.Num() -1)
    {
        CurrentWeaponIndex = 0;
    }
    WeaponAttachMethod(CurrentWeaponIndex);

    return CurrentWeaponIndex;
}

void ARPGPlayerCharacter::WeaponAttachMethod(int32 Slot)
{
    if(CurrentWeapon)
    {   
        CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    }
    
    CurrentWeapon = EquippedWeapons[Slot];
    CurrentWeaponSlot = CurrentWeapon->Slot;

    FName AttachSocket = (bIsCombat)? "GSEquip_R" : "GS_UnEquip";

    CurrentWeapon->AttachToComponent(this->GetMesh() , FAttachmentTransformRules::SnapToTargetNotIncludingScale , AttachSocket);
    
}

void ARPGPlayerCharacter::MeleeAttack()
{   
    if(!bIsCombat)
    {
        return;
    }

    //アニメーションの途中で更新されたときの対応
    UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
    if(!AnimInstance->GetCurrentActiveMontage())
    {
        ResetComboValue();
    }
    
    if(bEnableComboPeriod)
    {
        bSavingComboConnect = true;
    }
    else if(!bEnableComboPeriod && !bIsAttack && JumpSection == "Combo1")
    {   
        bIsAttack = true;
        TurnToTarget();
        PlayAnimMontage(CurrentAttackMontage , ComboPlayRate , "Combo1");
    }
    
}

void ARPGPlayerCharacter::ChargeAttackStart()
{   
    if(!ChargeMontage || !bIsCombat)
    {
        return;
    }
    bIsCharge = true;
    PlayAnimMontage(ChargeMontage , 1.1f , "Start");

    if(!ChargeLoopDelegate.IsBoundToObject(this))
    {
        ChargeLoopDelegate.BindUObject(this , &ARPGPlayerCharacter::ChargeAttackLoop);
    }

    GetWorldTimerManager().SetTimer(ChargeTimerHandle , ChargeLoopDelegate , 0.6f , true);
}

void ARPGPlayerCharacter::ChargeAttackLoop()
{
    ChargeCount++;

    if(!bIsMaxCharge && ChargeCount == 3)
    {
        bIsMaxCharge = true;
        
        MontageJumpSection(ChargeMontage , "Charge Comp");
    }
}

void ARPGPlayerCharacter::ChargeAttackEnd()
{
    if(bIsMaxCharge)
    {
        bIsMaxCharge = false;
    }
    else
    {
        MontageJumpSection(ChargeMontage , "Attack");
    }

    bIsCharge = false;
    ChargeCount = 0;
    GetWorldTimerManager().ClearTimer(ChargeTimerHandle);
    
}

//ローリングなどの途中介入があった時などに使用
void ARPGPlayerCharacter::ResetComboValue()
{
    bEnableComboPeriod = false;
    JumpSection = "Combo1";
    bIsAttack = false;
}

void ARPGPlayerCharacter::TurnToTarget()
{
    //近くに攻撃対象がいた場合、その方向を向く
    if(PreTarget != nullptr)
    {
        AActor* AttackTarget = (bLockedOnTarget)? TargetLockActor : PreTarget;
        FRotator TargetVec = UKismetMathLibrary::FindLookAtRotation(GetActorLocation() , AttackTarget->GetActorLocation());
        SetActorRotation(FRotator(0 , TargetVec.Yaw , TargetVec.Roll));
    }
}


////////////////////////////////////////////////////////
// Target
void ARPGPlayerCharacter::TargetLockEvent()
{  
   if(bLockedOnTarget)
   {   
       EndTargetEvent();
   }
   else
   {
        if(PreTarget == nullptr)
        {
            return;
        }

        bLockedOnTarget = !bLockedOnTarget;
        TargetLockActor = PreTarget;
        
        //WidgetComponentを有効に
        if(TargetLockActor->GetClass()->ImplementsInterface(URPGCharacterInterface::StaticClass()))
        {
            IRPGCharacterInterface::Execute_NotifyTarget(TargetLockActor , true);
        }
       
   }
   
}

void ARPGPlayerCharacter::EndTargetEvent()
{   
    IRPGCharacterInterface::Execute_NotifyTarget(TargetLockActor , false);
    bLockedOnTarget = false;
    TargetLockActor = nullptr;
}

void ARPGPlayerCharacter::SwitchTarget()
{
    if(bLockedOnTarget && (PreTarget != TargetLockActor))
    {   
        EndTargetEvent();
        TargetLockEvent();
    }
}

AActor* ARPGPlayerCharacter::CheckEnemy()
{   
    //サイトにオーバーラップされているアクターのチェック
    TArray<AActor*> OverlappingActors;
    PlayerSight->GetOverlappingActors(OverlappingActors , TSubclassOf<AActor>());
    TMap<AActor* , float> EnemyLengthMap;
    AActor* ClosestTarget = nullptr;
    float Len = 0.0f;

    if(!OverlappingActors.IsValidIndex(0))
    {   
        return nullptr;
    }
    
    for (AActor* Actor : OverlappingActors)
    {        
        ARPGEnemyBase* Enemy = Cast<ARPGEnemyBase>(Actor);
        if(Enemy && Enemy->IsAlive())
        {   
            EnemyLengthMap.Add(Enemy , GetDistanceTo(Enemy));
            
            ARPGEnemyMobBase* Mob = Cast<ARPGEnemyMobBase>(Enemy);
            if(Mob)
            {
                if( GetDistanceTo(Enemy) < 1200.f)
                {
                    Mob->UpdateBarVisibility(true);

                }
                else
                {
                    Mob->UpdateBarVisibility(false);
                }
            }
        }
    }
    
    for(TPair<AActor* , float>& Pair : EnemyLengthMap)
    {
        if(ClosestTarget == nullptr)
        {
            ClosestTarget = Pair.Key;
            Len = Pair.Value;
        }
        
        if(Len > Pair.Value)
        {   
            ClosestTarget = Pair.Key;
            Len = Pair.Value;
        }
    }


    return ClosestTarget;
}

FRotator ARPGPlayerCharacter::LookTargetDirection()
{   
    //Targetの方向を取得
    if(bLockedOnTarget || PreTarget)
    {
        AActor* Target = (bLockedOnTarget) ? TargetLockActor : PreTarget;
        FRotator DirectionToTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation() , Target->GetActorLocation());
        return DirectionToTarget;
    }
    else
    {
        return GetActorRotation();
    }
}


void ARPGPlayerCharacter::UseItem(int32 Index)
{
    if(!InventoryComp)
    {
        return;
    }
    
    URPGItem* Item = InventoryComp->GetSlottedItem(FRPGItemSlot("Potion" , Index));
    URPGPotionItem* PotionItem = Cast<URPGPotionItem>(Item);
    float EffectItem = 0.f;                ///評価項目の最大値
    float CurrentEffectValue = 0.f;        ///現在の評価項目の値
    bool bHUDUpdate = false;
    FTimerHandle TimeHandle;

    if(PotionItem)
    {   
        PlayAnimMontage(PotionItem->ItemUseMontage);

        switch(PotionItem->EffectType)
        {
            case ERPGStatusType::HP :
                EffectItem = ParentStatus.HP;
                CurrentEffectValue = CharStatus.HP;
                bHUDUpdate = true;
                break;
            
            case ERPGStatusType::MP :
                EffectItem = ParentStatus.MP;
                CurrentEffectValue = CharStatus.MP;
                bHUDUpdate = true;
                break;

            case ERPGStatusType::Attack :
                EffectItem = ParentStatus.Attack;
                CurrentEffectValue = CharStatus.Attack;
                break;

            case ERPGStatusType::Vitality :
                EffectItem = ParentStatus.Vitality;
                CurrentEffectValue = CharStatus.Vitality;
                break;

            case ERPGStatusType::Agility :
                EffectItem = ParentStatus.Agility;
                CurrentEffectValue = CharStatus.Agility;
                break;

            default :
                break;
        }

        float EffectValue = CurrentEffectValue + EffectItem * PotionItem->EffectValue;
        
        if(PotionItem->EffectType == ERPGStatusType::HP || PotionItem->EffectType == ERPGStatusType::MP)
        {   
            bool IsItemHP = PotionItem->EffectType == ERPGStatusType::HP;
            FloatDamageText(EffectItem * PotionItem->EffectValue , this , false , IsItemHP);
            EffectValue = FMath::Clamp(EffectValue , 0.f , EffectItem);
        }

        CharStatus.CustumizeUpdate(PotionItem->EffectType , EffectValue);
        if(PlayerController->HUD && bHUDUpdate)
        {
            IRPGUIInterface::Execute_UpdateHP(PlayerController->HUD , CharStatus.HP / ParentStatus.HP);
        }
        
        InventoryComp->RemoveInventoryItem(PotionItem , 1);
        
        if(PotionItem->EffectTime > 0.f)
        {
            GetWorldTimerManager().SetTimer(TimeHandle , [=](){
                CharStatus.CustumizeUpdate(PotionItem->EffectType , EffectItem);
                this->GetMesh()->SetVectorParameterValueOnMaterials("BodyEffect" , FVector(BaseEffectColor));
            } , PotionItem->EffectTime , false);
        }
    }

}

///////////////////////////////////////////////////////////////////
//   Interface

void ARPGPlayerCharacter::OnDamaged_Implementation(ARPGCharacterBase* DamageCauser  , float Damage)
{   
    if(bIsDead)
    {
        return;
    }

    FTimerHandle Handle;

    this->SetActorRotation(FRotator(0.f , UKismetMathLibrary::FindLookAtRotation( this->GetActorLocation() , DamageCauser->GetActorLocation()).Yaw ,0.f));
    
    float VectorCoefficient = (bIsGuard) ? 0.5f : 1.f;
    FVector LaunchVelocity = (-1.0f)*GetActorForwardVector() * VectorCoefficient * 1000.f; 
    this->LaunchCharacter(LaunchVelocity , true , true);

    //ガードしていないとき
   
    CharStatus.HP -= VectorCoefficient * Damage;
    FloatDamageText(Damage , this);
    
    if(PlayerController->HUD )
    {   
        if(PlayerController->HUD->GetClass()->ImplementsInterface(URPGUIInterface::StaticClass()))
        {
            
            IRPGUIInterface::Execute_UpdateHP(PlayerController->HUD , CharStatus.HP / ParentStatus.HP);

            if(CharStatus.HP <= 0.f)
            {
                UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
                if(AnimInstance->IsAnyMontagePlaying())
                {
                    StopAnimMontage(AnimInstance->GetCurrentActiveMontage());
                }
                
                
                bIsDead = true;
                
                DisableInput(PlayerController);

                this->GetCharacterMovement()->DisableMovement();
                GetWorldTimerManager().ClearTimer(SearchHandle);
                GetWorldTimerManager().SetTimer(Handle , [=](){
                    PlayerController->GameOver();
                    EnableInput(PlayerController);
                } , 3.0f , false);
               
            }
            else
            {
                bIsDamaged = true;

                if(HitReactMotion)
                {   
                    FName SectionName = (bIsGuard) ? "Guard" : "Arm";
                    PlayAnimMontage(HitReactMotion , 1.0f , SectionName);
                    GetWorldTimerManager().SetTimer(Handle , [this](){
                        bIsDamaged = false;
                    }, 1.0f , false);
                }
            }

        }
        
    }

   
   
}

void ARPGPlayerCharacter::DefeatEnemy_Implementation(ARPGEnemyBase* Enemy)
{  
    CharStatus.Exp -= Enemy->DefeatExp;
    float RemainExp = (CharStatus.Exp < 0) ? -CharStatus.Exp : 0.f;

    if(FMath::Clamp(CharStatus.Exp , 0.f , ParentStatus.Exp) == 0)
    {   
        if(StatusComp)
        {   
           
            StatusComp->LevelUp(ParentStatus);
            CharStatus = ParentStatus;

            if(OnLevelUp.IsBound())
            {
                OnLevelUp.Broadcast(ParentStatus.CharLevel);
            }
        }
        
    }
    
    
    CharStatus.Exp -= RemainExp;
    if(TargetLockActor == Enemy)
    {   
       EndTargetEvent();     
    }

}