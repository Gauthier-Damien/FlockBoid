// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"


USTRUCT()
struct FBoidParams
{
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, Category = Global)
		float speed = 100;
	UPROPERTY(EditAnywhere, Category = Global)
		float speedVariation = 10;
	UPROPERTY(EditAnywhere, Category = Global)
		float flockRadius = 300;
	UPROPERTY(EditAnywhere, Category = Global)
		int bounds = 2000;
	UPROPERTY(EditAnywhere, Category = Global)
		bool snapToZero = false;

	UPROPERTY(EditAnywhere, Category = Separation)
		float separationRadius = 200;
	UPROPERTY(EditAnywhere, Category = Separation)
		float separationStrength= 1000;

	UPROPERTY(EditAnywhere, Category = Alignment)
		float alignmentRadius = 200;
	UPROPERTY(EditAnywhere, Category = Alignment)
		float alignmentStrength = 1000;


	UPROPERTY(EditAnywhere, Category = Coherence)
		float coherencetRadius = 200;
	UPROPERTY(EditAnywhere, Category = Coherence)
		float coherenceStrength = 1000;

};

UCLASS()
class FLOCKBOID_API ABoidManager : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = Parameters)
		FBoidParams boidParameters = FBoidParams();
public:	




	ABoidManager() = default;

#pragma region sensor

protected:
	UPROPERTY(EditAnywhere, Category = "Avoidance", meta = (ClampMin = "0", ClampMax = "1000"))
		int32 numSensors;
	const float goldenRatio = (1.0f + FMath::Sqrt(5.0f)) / 2;
	UPROPERTY(EditAnywhere, Category = "Avoidance", meta = (ClampMin = "0"))
		float sensorRadius;
	TArray<FVector> avoidanceSensors;
	void BuildAvoidanceSensors();

public:
	//getters + setters
	inline float GetSensorRadius() { return sensorRadius; }
	inline TArray<FVector> GetAvoidanceSensors() { return avoidanceSensors; }

#pragma endregion


	FBoidParams GetBoidParameters() { return boidParameters; }

};
