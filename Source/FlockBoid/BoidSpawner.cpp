#include "BoidSpawner.h"

void ABoidSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPos = bEnableCenterSpawn ? GetRandomVector(-SpawnArea, SpawnArea) : FVector::ZeroVector;
		FRotator RandRotation(0, FMath::RandRange(-180, 180), 0);
		ABoid* NewBoid = GetWorld()->SpawnActor<ABoid>(ActorToSpawn, SpawnPos, RandRotation);
	}
	for (size_t i = 0; i < 1; i++)
	{
		FVector SpawnPos = bEnableCenterSpawn ? GetRandomVector(-SpawnArea, SpawnArea) : FVector::ZeroVector;
		FRotator RandRotation(0, FMath::RandRange(-180, 180), 0);
		ABoid* NewBoid = GetWorld()->SpawnActor<ABoid>(ActorToSpawn, SpawnPos, RandRotation);
		NewBoid->SetAsLeader();
	}


}


