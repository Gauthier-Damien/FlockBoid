#include "BoidDataAsset.h"
#include "Kismet/KismetMathLibrary.h"

void UBoidDataAsset::BuildAvoidanceSensors()
{
	AvoidanceSensors.Empty();

	float Theta;
	float Phi;
	FVector SensorDirection;

	for (int32 i = 0; i < NumSensors; ++i)
	{
		Theta = 2 * UKismetMathLibrary::GetPI() * GOLDEN_RATIO * i;
		Phi = FMath::Acos(1 - (2 * float(i) / NumSensors));

		SensorDirection.X = FMath::Cos(Theta) * FMath::Sin(Phi);
		SensorDirection.Y = FMath::Sin(Theta) * FMath::Sin(Phi);
		SensorDirection.Z = FMath::Cos(Phi);



		AvoidanceSensors.Emplace(SensorDirection);
	}
}
