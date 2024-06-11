#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoidDataAsset.generated.h"


#define GOLDEN_RATIO (1.0f + FMath::Sqrt(5.0f)) / 2


UCLASS()
class FLOCKBOID_API UBoidDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

#pragma region GlobalProps
	UPROPERTY(EditAnywhere, Category = Global, meta = (ClampMin = "0"))
		float Speed = 100;
	UPROPERTY(EditAnywhere, Category = Global, meta = (ClampMin = "0"))
		float SpeedVariation = 10;
	UPROPERTY(EditAnywhere, Category = Global, meta = (ClampMin = "0"))
		float FlockRadius = 300;
	UPROPERTY(EditAnywhere, Category = Global)
		int BoundSize = 2000;
	UPROPERTY(EditAnywhere, Category = Global)
		int timeBeforeLeaderSwitch = 5;
	UPROPERTY(EditAnywhere, Category = Global)
		int NumberForLeaderFlock = 10;

	UPROPERTY(EditAnywhere, Category = Global)
		int Center = 0;
	UPROPERTY(EditAnywhere, Category = Global)
		bool bSnapToZero = false;
	UPROPERTY(EditAnywhere, Category = Debug)
		bool DrawDebug = false;
	UPROPERTY(EditAnywhere, Category = Debug)
	FColor DefaultTrailColor = FColor::White;

#pragma endregion GlobalProps

#pragma region SeparationProps
	UPROPERTY(EditAnywhere, Category = Separation , meta = (ClampMin = "0"))
		float SeparationRadius = 200;
	UPROPERTY(EditAnywhere, Category = Separation, meta = (ClampMin = "0"))
		float SeparationStrength = 1000;
#pragma endregion SeparationProps

#pragma region AlignmentProps
	UPROPERTY(EditAnywhere, Category = Alignment, meta = (ClampMin = "0"))
		float AlignmentRadius = 200;
	UPROPERTY(EditAnywhere, Category = Alignment, meta = (ClampMin = "0"))
		float AlignmentStrength = 1000;
#pragma endregion AlignmentProps


#pragma region CoherenceProps
	UPROPERTY(EditAnywhere, Category = Coherence, meta = (ClampMin = "0"))
		float CoherenceRadius = 200;
	UPROPERTY(EditAnywhere, Category = Coherence, meta = (ClampMin = "0"))
		float CoherenceStrength = 1000;
#pragma endregion CoherenceProps
#pragma region sensor
	UPROPERTY(EditAnywhere, Category = Avoidance, meta = (ClampMin = "0"))
		float AvoidanceStrength = 200;
protected:
	UPROPERTY(EditAnywhere, Category = Avoidance, meta = (ClampMin = "0", ClampMax = "1000"))
		int32 NumSensors;
	UPROPERTY(EditAnywhere, Category = Avoidance, meta = (ClampMin = "0"))
		float SensorRadius;
	TArray<FVector> AvoidanceSensors;
	void BuildAvoidanceSensors();

public:
	inline float GetSensorRadius() { return SensorRadius; }
	inline TArray<FVector> GetAvoidanceSensors() { return AvoidanceSensors; }

#pragma endregion

};
