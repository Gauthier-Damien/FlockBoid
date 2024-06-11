// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.h"
#include "BoidManager.h"
#include "BoidSpawner.generated.h"



UCLASS()
class FLOCKBOID_API ABoidSpawner : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Spawner)
		int NumberToSpawn = 10;
	UPROPERTY(EditAnywhere, Category = Spawner)
		TSubclassOf<ABoid> ActorToSpawn = ABoid::StaticClass();
	UPROPERTY(EditAnywhere)
		int SpawnArea = 2000;
	UPROPERTY(EditAnywhere, Category = Spawner)
		bool bEnableCenterSpawn = true;

public:	
	ABoidSpawner() = default;

protected:
	virtual void BeginPlay() override;
	FVector GetRandomVector(double Min, double Max)
	{
		return FVector(FMath::RandRange(Min, Max), FMath::RandRange(Min, Max), FMath::RandRange(Min, Max));
	}
	
};
