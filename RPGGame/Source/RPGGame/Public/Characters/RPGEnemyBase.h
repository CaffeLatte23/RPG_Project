// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "RPGTypes.h"
#include "Characters/RPGCharacterBase.h"
#include "Interface/RPGCharacterInterface.h"
#include "Dungeon/EnemySpawnerVolume.h"
#include "RPGEnemyBase.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnSpawnFinished);

UCLASS()
class RPGGAME_API ARPGEnemyBase : public ARPGCharacterBase
{
	GENERATED_BODY()

	FTimeline* Timeline;

public: 
    
	ARPGEnemyBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
    
	UPROPERTY()
	class ARPGPlayerCharacter* Player;

	UPROPERTY(VisibleAnywhere , meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* LockWC;

	UPROPERTY(EditDefaultsOnly , Category = "Default")
	TArray <class URPGItem*> WeaponItemArray;

	UPROPERTY()
	class URPGItem* WeaponItem;

	UPROPERTY( EditDefaultsOnly , BlueprintReadOnly , Category = "Default")
	FName CharName;
    
    FTimerHandle Handle;

	UPROPERTY(EditDefaultsOnly , Category = "Default")
	bool bEnableSpawnEffect = true;

	UPROPERTY(EditDefaultsOnly , Category = "Default")
	class UParticleSystem* SpawnEffect;

	UPROPERTY()
	class UParticleSystem* DestoryEffect;

	UPROPERTY(EditDefaultsOnly , Category = "Default")
	float FinalScale = 1.0f;

	FOnSpawnFinished OnSpawnFinished;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = "Attack")
	TArray<class UAnimMontage*> CurrentAttackMontage;

	UPROPERTY(BlueprintReadOnly , Category = AI)
	AEnemySpawnerVolume* ParentVolume = nullptr;
    
	//スポーン時のエフェクト処理
	UFUNCTION()
	void OnSpawning();

	UFUNCTION()
	void OnSpawnFinish();

	UFUNCTION()
	void UpdateRelativeSize(FVector Value);

	UFUNCTION(BlueprintCallable , Category = AI)
	void UpdateCombatType(ECombatType NewType);

//////////////////////////////////////////////////////////////////////
// Collision(武器がない場合、武器がメッシュについている場合)

    UPROPERTY(EditDefaultsOnly , Category = "Weapon")
	float BaseDamage = 10.f;

    UPROPERTY()
	TArray<AActor*> DamagedActors;

	UPROPERTY()
	class UParticleSystem* HitEffect;

	UPROPERTY()
	class USoundBase* HitSound;

    //ベースのコリジョン
	UPROPERTY(VisibleAnywhere , meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Collision_R;

	UPROPERTY(VisibleAnywhere , meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Collision_L;

    //AnimNotify 呼び出し用
	UPROPERTY()
	TMap<EEnableCollisionPoint ,  UCapsuleComponent*> CollisionMap;

	UFUNCTION(BlueprintCallable , Category = "Collsion")
	void EnableCollision(EEnableCollisionPoint CollisionPoint);

	UFUNCTION(BlueprintCallable , Category = "Collsion")
	void DisableCollision(EEnableCollisionPoint CollisionPoint);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


//////////////////////////////////////////////////////////////////////
//// InDefeat 

	UPROPERTY(EditDefaultsOnly , Category = "Drop")
	float DefeatExp = 5.f;

	UPROPERTY(EditAnywhere  , BlueprintReadWrite , Category = "AI")
	ECombatType CombatType;
    
    UPROPERTY(BlueprintReadWrite , Category = "Combo")
	int32 CurrentCombo = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsStun = false;

	UFUNCTION(BlueprintCallable , Category = "AI")
	virtual void Attack();

	UFUNCTION()
	void SpawnWeapon();

	UFUNCTION(BlueprintCallable , Category = "Combo")
	void HitStopHandle();

	UFUNCTION()
	void ApplyDamage(AActor* OtherActor);

	UFUNCTION()
    virtual void NotifyTarget_Implementation(bool bTarget) override;

};



