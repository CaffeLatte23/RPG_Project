// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "RPGTypes.h"
#include "Characters/RPGCharacterBase.h"
#include "RPGPlayerCharacter.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp , int32 , NewLv );



UCLASS()
class RPGGAME_API ARPGPlayerCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlayerSight;

	FTimeline* Timeline;

public:
    
	ARPGPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class ARPGPlayerControllerBase* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , meta = (AllowPrivateAccess = "true"))
	class URPGInventoryComponent* InventoryComp;

	/*横向きのカメラスピード*/
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite ,  Category=Camera)
	float BaseTurnRate = 45.f;

	/*上向きのカメラスピード*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,  Category=Camera)
	float BaseLookUpRate = 45.f;

	FLinearColor BaseEffectColor = FLinearColor( 1.f , 0.142913f , 0.023219f , 0.f);
    
	UPROPERTY(BlueprintAssignable , Category = Status)
	FOnLevelUp OnLevelUp;

protected:
    
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);

	void MoveRight(float Value); 

	void TurnAtRate(float Rate);

	void LookAtRate(float Rate);

	void StopJumping();

	void DashAction();
    
	UFUNCTION(BlueprintCallable , Category = Combat)
	void EndSprint();

	UFUNCTION()
	void GuardAction();
    
	//Timeline用のupdate関数　UFUNCTION()が必須
	UFUNCTION()
	void UpdateMoveSpeed(float Value);
    
	void CameraMove();

	void UpdateCameraType(bool bTarget);

public:	
	
	//着地時の受け身のトリガー
	UPROPERTY()
	bool bDynamicFall = false;
    
	//ダッシュ、回避状態にあるか
	UPROPERTY()
	bool bIsDash = false;

	UPROPERTY()
    bool bMPAnyChange = false; 

	UPROPERTY(BlueprintReadWrite)
	bool bMoveCheck = false;
    
	//ジャストガードもしくは回避のフラグ
	UPROPERTY(BlueprintReadWrite)
	bool bEnableJustGuard = false;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combo)
	UAnimMontage* RollingMontage;

	UFUNCTION(BlueprintCallable , Category = Combat)
	bool CombatCheck();

	UFUNCTION()
	void Slot1ItemUse();

	UFUNCTION()
	void Slot2ItemUse();

	UFUNCTION()
	void RecoveryMPStart();

	UFUNCTION()
	void RecoveryMP();


	EDirectionType GetInputDirection();

//////////////////////////
//Weapon
    UPROPERTY()
	int32 CurrentWeaponIndex = -1;

	UPROPERTY()
	bool bFirstAttach = true;

	//攻撃モーション変数
	UPROPERTY(EditAnywhere ,BlueprintReadOnly , Category = "Default")
	class UAnimMontage* CurrentAttackMontage;
    
    UPROPERTY()
	TArray<class URPGItem*> EquippedRPGItems;

	UPROPERTY()
	TArray<class ARPGWeaponBase*> EquippedWeapons;
    
    //装備イベント
	UFUNCTION()
	void EquipAction();

	//武器生成イベント
	UFUNCTION(BlueprintCallable , Category = "Weapon")
	void CreateAllWeapons();
    
	//アタッチのイベント
    UFUNCTION()
	int32 AttachNextWeapon();

	UFUNCTION()
	void WeaponAttachMethod(int32 Slot);

	UFUNCTION()
	void MeleeAttack();

	UFUNCTION()
	void ChargeAttackStart();

	UFUNCTION()
	void ChargeAttackLoop();

	UFUNCTION()
	void ChargeAttackEnd();

//////////////////////////
// Combo

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combo)
	FName JumpSection = "Combo1";


	UPROPERTY(BlueprintReadWrite , Category = Combo)
	bool bSavingComboConnect = false;

	FTimerHandle ChargeTimerHandle;

	FTimerDelegate ChargeLoopDelegate;

	UPROPERTY()
	int32 ChargeCount = 0;

	UPROPERTY()
	bool bIsCharge = false;

	UPROPERTY()
	bool bIsMaxCharge = false;

	UFUNCTION(BlueprintCallable , Category = "Combo")
	void ResetComboValue();

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Default")
	class UAnimMontage* ChargeMontage;


//////////////////////////
// Target

    //ターゲットを固定しているか
	UPROPERTY(BlueprintReadOnly)
	bool bLockedOnTarget = false;
	
    UPROPERTY(BlueprintReadOnly)
    AActor* TargetLockActor;
    
	UPROPERTY(BlueprintReadOnly)
	AActor* PreTarget;
    
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle SearchHandle;

	UFUNCTION(BlueprintPure , BlueprintCallable ,  Category = "Target")
	FRotator LookTargetDirection();

	UFUNCTION()
	void TargetLockEvent();

	UFUNCTION()
	void EndTargetEvent();

	UFUNCTION()
	void SwitchTarget();

	UFUNCTION()
	AActor* CheckEnemy();

/////////////////////////////
//  Item
	UFUNCTION(BlueprintCallable , Category = Item)
	void UseItem(int32 Index);

/////////////////////////////
// Interface

	UFUNCTION()
	virtual void OnDamaged_Implementation(ARPGCharacterBase* DamageCauser  , float Damage) override;

	UFUNCTION()
	virtual void DefeatEnemy_Implementation(class ARPGEnemyBase* Enemy) override;

private : 
    
	FTimerHandle SprintTimeHandle;
};





