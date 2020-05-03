// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "GameFramework/Actor.h"
#include "RPGWeaponBase.generated.h"

UCLASS()
class RPGGAME_API ARPGWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGWeaponBase();

	UPROPERTY(VisibleAnywhere , meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Root;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly ,  meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere , meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* WeaponCollision;

	UPROPERTY(EditAnywhere ,BlueprintReadOnly , Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(BlueprintReadOnly , Category = "Weapon")
	float BaseDamage = 20.f;

	UPROPERTY(BlueprintReadOnly , Category = "Weapon")
	float CriticalDamage = 10.f;



	UPROPERTY()
	FName OwnerTag = "";

	UPROPERTY()
	TArray<AActor*> DamagedActor;
    
	UPROPERTY(BlueprintReadOnly)
	FRPGItemSlot Slot;

	UPROPERTY(BlueprintReadOnly)
	class URPGWeaponItem* Item;

	UFUNCTION()
	void SetWeaponSpec();

	UFUNCTION(BlueprintCallable , Category = "Collision")
	void CollisionAble();

	UFUNCTION(BlueprintCallable , Category = "Collision")
	void CollisionDisable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
