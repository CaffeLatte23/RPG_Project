// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGSpawnPoint.generated.h"

UCLASS()
class RPGGAME_API ARPGSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGSpawnPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* BillBoard;

	UPROPERTY()
	bool bSpawnEnable = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
