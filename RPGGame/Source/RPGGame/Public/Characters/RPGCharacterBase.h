// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "RPGTypes.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/RPGStatusComponent.h"
#include "Weapon/RPGWeaponBase.h"
#include "Blueprint/UserWidget.h"
#include "Animation/AnimInstance.h"
#include "Interface/RPGCharacterInterface.h"
#include "RPGCharacterBase.generated.h"

UCLASS()
class RPGGAME_API ARPGCharacterBase : public ACharacter , public IRPGCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly ,   meta = (AllowPrivateAccess = "true"))
	URPGStatusComponent* StatusComp;

	UPROPERTY(EditAnywhere , Category = Combat)
	float BaseSpeed = 600.f;

	UPROPERTY(EditAnywhere , Category = Combat)
	float MaxSpeed = 800.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
	//攻撃中か
    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat)
	bool bIsAttack = false;

	//戦闘状態にあるか
    UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat)
	bool bIsCombat = false;
    
	//ガード状態か
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat)
	bool bIsGuard = false;

    //ガードヒット
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat)
	bool bGuardHit = false;
    
	//死亡判定
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Combat)
	bool bIsDead = false;
    
	//ダメージ判定
	UPROPERTY(BlueprintReadWrite)
	bool bIsDamaged = false;
    
	//コンボ接続フラグ
	UPROPERTY(BlueprintReadWrite , Category = Combo)
	bool bEnableComboPeriod = false;

	UPROPERTY(BlueprintReadWrite ,  Category = Status)
	FRPGStatus CharStatus;

	UPROPERTY(EditAnywhere  , Category = Combo)
	float ComboPlayRate = 1.0f;

	UPROPERTY(BlueprintReadOnly , Category = Weapon)
	class ARPGWeaponBase* CurrentWeapon = nullptr;

	FRPGItemSlot CurrentWeaponSlot;
    
	//装備モーション
	UPROPERTY(EditAnywhere , Category = Weapon)
	class UAnimMontage* EquipMontage;

	//死亡モーション
	UPROPERTY(EditDefaultsOnly , Category = Default)
	class UAnimMontage* HitReactMotion;

	//ダメージ計算
	UFUNCTION()
	float CalclateDamage(float BaseDamage , float CriDamage );
    
	UPROPERTY(BlueprintReadWrite , Category = "Combat")
	float MinDamagefactor = 1.0f;

	UPROPERTY(BlueprintReadWrite , Category = "Combat")
	float MaxDamagefactor = 1.0f;

	UPROPERTY(BlueprintReadOnly , Category = "UI")
	TSubclassOf<UUserWidget> FloatingTextClass;

	UFUNCTION(BlueprintCallable , Category = "UI")
	void FloatDamageText(float Damage , AActor* HitActor);

	//生存確認
	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	UFUNCTION(BlueprintPure , BlueprintCallable , Category = "Status")
	FRPGStatus GetOwnerStatus();
    
	//ソケットへのアタッチ　共通処理
	UFUNCTION(BlueprintCallable , Category = "Weapon")
	void Attach(FName SocketName , bool bCombatChange);

	UFUNCTION(BlueprintCallable , Category = "Montage")
	void MontageJumpSection(UAnimMontage* TargetMontage , FName SectionName);
    
	//エネミー用のデフォルトレベル変数（スポーンエリアで編集）
	UPROPERTY(EditAnywhere , Category = "Default")
	int32 DefaultLevel = 1;

    EDirectionType GetTargetDirection(ARPGCharacterBase* Base , ARPGCharacterBase* Target);

};
