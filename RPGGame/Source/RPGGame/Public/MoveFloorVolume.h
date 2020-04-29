// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveFloorVolume.generated.h"

UCLASS()
class RPGGAME_API AMoveFloorVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveFloorVolume();
    
	//仮のスタティックメッシュ
	UPROPERTY()
	class UStaticMeshComponent*  Mesh;

	UPROPERTY()
	class UBoxComponent* MoveVolume;

   	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
};
