

#include "BoidManager.h"
#include "Kismet/KismetMathLibrary.h"

void ABoidManager::BuildAvoidanceSensors()
{
	avoidanceSensors.Empty();

	float theta;
	float phi;
	FVector SensorDirection;

	for (int32 i = 0; i < numSensors; ++i)
	{
		theta = 2 * UKismetMathLibrary::GetPI() * goldenRatio * i;
		phi = FMath::Acos(1 - (2 * float(i) / numSensors));

		SensorDirection.X = FMath::Cos(theta) * FMath::Sin(phi);
		SensorDirection.Y = FMath::Sin(theta) * FMath::Sin(phi);
		SensorDirection.Z = FMath::Cos(phi);



		avoidanceSensors.Emplace(SensorDirection);
	}
}
