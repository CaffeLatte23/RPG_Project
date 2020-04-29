// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPGGame.h"
#include "GameFramework/Actor.h"
#include "ItemPickUp.generated.h"

UCLASS()
class RPGGAME_API AItemPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

    UPROPERTY(VisibleAnywhere , BlueprintReadOnly ,   meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Root;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly ,   meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollectCollision;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = Default)
	FLinearColor Color;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = Default)
	class URPGItem* ItemType;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = Default)
	int32 Count = 0;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = Default)
	FVector Impulse;

	UPROPERTY(BlueprintReadOnly , Category = Default)
	AActor* PlayerCollecting;

	UPROPERTY(EditDefaultsOnly , Category = PickUp)
	USoundBase* PickUpSound;
    
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable , Category = PickUp)
	void AnimationCollection();

	UFUNCTION()
	void GiveItem();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
