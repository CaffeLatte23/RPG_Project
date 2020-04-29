// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacterBase.h"


// Sets default values
ARPGCharacterBase::ARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	//カメラの回転に合わせないようにする
	bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

	//CharacterMovement Setting
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,540.f,0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
  
	//ステータスコンポーネントの追加
	StatusComp = CreateDefaultSubobject<URPGStatusComponent>(TEXT("StatusComponent"));

}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void ARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

// Called every frame
void ARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCharacterBase::Attach(FName SocketName , bool bChangeCombat)
{
	CurrentWeapon->AttachToComponent(this->GetMesh() ,  FAttachmentTransformRules::SnapToTargetNotIncludingScale , SocketName );

	if(bChangeCombat)
	{
		bIsCombat = !bIsCombat;
	}
}

void ARPGCharacterBase::MontageJumpSection(UAnimMontage* TargetMontage , FName SectionName)
{
    UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	if(AnimInstance->GetCurrentActiveMontage() == TargetMontage)
	{   
		AnimInstance->Montage_JumpToSection(SectionName , TargetMontage);
	}
}

float ARPGCharacterBase::CalclateDamage(float BaseDamage , float CriDamage , float CriPercent) 
{
	float Base = FMath::RandRange(BaseDamage - CriDamage , BaseDamage);
	float UseRate = FMath::RandRange( 0.f , CriPercent);

	return Base + (int32)(CriDamage * UseRate);
}

bool ARPGCharacterBase::IsAlive()
{   
	return CharStatus.HP > 0;
}

EDirectionType ARPGCharacterBase::GetTargetDirection(ARPGCharacterBase* Base , ARPGCharacterBase* Target)
{   
	//当たった時の回転の差を,180度で出力
	float BaseZ = Base->GetActorRotation().Yaw;
	float TargetZ = Target->GetActorRotation().Yaw;
    
	float NormalValue = UKismetMathLibrary::NormalizeToRange(BaseZ - TargetZ , -180.f , 180.f);

	if(NormalValue <= 112.5f && NormalValue >= 67.5f)
	{
		return EDirectionType::Behind;
	}
	else if(NormalValue >= 22.5 && NormalValue < 67.5)
	{
		return EDirectionType::Right;
	}
	else if(NormalValue > 112.5 && NormalValue <= 157.5)
	{
        return EDirectionType::Left; 
	}
	else
	{
		return EDirectionType::Forward;
	}
	
}





