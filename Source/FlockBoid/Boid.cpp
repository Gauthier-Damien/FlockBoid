#include "Boid.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BoidManager.h"

#define COLLISION_AVOIDANCE				ECC_GameTraceChannel1

ABoid::ABoid()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABoid::BeginPlay()
{
	Super::BeginPlay();

	BoidParams = BoidParamsSbire;
	TrailColor = BoidParams->DefaultTrailColor;
	TrailFX = GetComponentByClass<UNiagaraComponent>();


	GetWorldTimerManager().SetTimer(Timer, this, &ABoid::CheckLeadership, BoidParams->timeBeforeLeaderSwitch, true, false);



	InitColliderSphere();

	Velocity = (GetActorForwardVector().GetSafeNormal()) * BoidParams->Speed;
	
}

void ABoid::InitColliderSphere()
{
	SphereCollider = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), "Sphere");

	if (SphereCollider)
	{
		SphereCollider->SetupAttachment(RootComponent);
		SphereCollider->RegisterComponent();
		SphereCollider->CreationMethod = EComponentCreationMethod::Instance;
		SphereCollider->SetSphereRadius(BoidParams->FlockRadius);

	}

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABoid::AddNewActorInList);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &ABoid::RemoveActorInList);
}


void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Steer(DeltaTime);

	if(BoidParams->DrawDebug)
		DrawDebug();
}

void ABoid::SetAsLeader()
{
	BoidRole = [&]() ->EBoidModificator {  SetTrailColor(FColor::MakeRandomColor()); TrailFX->SetFloatParameter("JitterAmount", 0); BoidParams = BoidParamsLeader; return EBoidModificator::leader;  }();
}

void ABoid::Steer(float DeltaTime)
{
	FVector Acceleration = {};



	Acceleration += Separate(Flock);
	Acceleration += Alignment(Flock);
	Acceleration += Coherence(Flock);

	if (BoidParams->bSnapToZero)
	{
		Acceleration.Z = 0;
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0));
	}


	Velocity += Acceleration * DeltaTime;
	Velocity = Velocity.GetClampedToSize(BoidParams->Speed - BoidParams->SpeedVariation, BoidParams->Speed + BoidParams->SpeedVariation);
	
	SetActorLocation(GetActorLocation() + (Velocity * DeltaTime));
	SetActorRotation(Velocity.ToOrientationQuat());
	StayInBounds();
}

void ABoid::StayInBounds()
{
	FVector CurrentLoc = GetActorLocation();

	int Bounds = BoidParams->BoundSize;
	if (CurrentLoc.X < -Bounds)
		CurrentLoc.X =  Bounds;
	if (CurrentLoc.X >  Bounds)
		CurrentLoc.X = -Bounds;

	if (CurrentLoc.Y < -Bounds)
		CurrentLoc.Y = Bounds;
	if (CurrentLoc.Y > Bounds)
		CurrentLoc.Y = -Bounds;

	if (CurrentLoc.Z < -Bounds)
		CurrentLoc.Z = Bounds;
	if (CurrentLoc.Z >  Bounds)
		CurrentLoc.Z = -Bounds;

	SetActorLocation(CurrentLoc);

}

FVector ABoid::Separate(TArray<ABoid*> Boids)
{
	FVector Steering = {};

	FVector SeparationDir = {};

	int FlockNum = 0;

	for (ABoid* b : Boids)
	{
		if (b && b != this)
		{
			float Dist = FVector::Distance(GetActorLocation(), b->GetActorLocation());
			if (Dist > BoidParams->SeparationRadius)
				continue;


			SeparationDir = (GetActorLocation() - b->GetActorLocation()).GetSafeNormal();
			Steering += SeparationDir;

			FlockNum++;
		}


		if (FlockNum && Cast<ABoid>(b)->BoidRole == EBoidModificator::leader)
		{
			Steering /= FlockNum;
			Steering = Steering.GetSafeNormal();
			Steering -= Velocity.GetSafeNormal();
			Steering *= BoidParams->SeparationStrength;
			return Steering / 10;

		}

	}

	if (FlockNum > 0)
	{
		Steering /= FlockNum;
		Steering = Steering.GetSafeNormal();
		Steering -= Velocity.GetSafeNormal();
		Steering *= BoidParams->SeparationStrength;
		return Steering;

	}

	return {};
}

FVector ABoid::Alignment(TArray<ABoid*> Boids)
{
	FVector Steering = {};


	int FlockNum = 0;

	for (ABoid* b : Boids)
	{
		if (b && b != this)
		{
			float Dist = FVector::Distance(GetActorLocation(), b->GetActorLocation());
			if (Dist > BoidParams->AlignmentRadius)
				continue;

			Steering += b->Velocity.GetSafeNormal();
			Steering = Steering.GetSafeNormal();

			FlockNum++;
		}
	
		if (FlockNum > 0 && Cast<ABoid>(b)->BoidRole == EBoidModificator::leader)
		{
			Steering = b->Velocity.GetSafeNormal(); ;
			Steering.GetSafeNormal() -= Velocity.GetSafeNormal();
			Steering *= BoidParams->AlignmentStrength;
			return Steering * 10;
		}


	}

	if (FlockNum > 0)
	{
		Steering /= FlockNum;
		Steering.GetSafeNormal() -= Velocity.GetSafeNormal();
		Steering *= BoidParams->AlignmentStrength;
		return Steering;
	}
	return {};

}

FVector ABoid::Coherence(TArray<ABoid*> Boids)
{
	FVector Steering = {};
	FVector AveragePosition = {};

	int FlockNum = 0;

	for (ABoid* b : Boids)
	{
		if (b && b != this)
		{
			float Dist = FVector::Distance(GetActorLocation(), b->GetActorLocation());
			if (Dist > BoidParams->CoherenceRadius)
				continue;
			
			AveragePosition += b->GetActorLocation();

			FlockNum++;
		}

		if (FlockNum > 0 && Cast<ABoid>(b)->BoidRole == EBoidModificator::leader)
		{
		
			AveragePosition = b->GetActorLocation();
			Steering = AveragePosition - GetActorLocation();
			Steering = Steering.GetSafeNormal();
			Steering -= Velocity.GetSafeNormal();
			Steering *= BoidParams->CoherenceStrength;
			return Steering * 20;
		}


	}
	if (FlockNum > 0)
	{
		AveragePosition /= FlockNum;
		Steering = AveragePosition - GetActorLocation();
		Steering = Steering.GetSafeNormal();
		Steering -= Velocity.GetSafeNormal();
		Steering *= BoidParams->CoherenceStrength;
		return Steering;
	}

	return {};
}

void ABoid::CheckLeadership()
{
	
	BoidRole = Flock.Num() > BoidParams->NumberForLeaderFlock ?
		[&]() ->EBoidModificator {  SetTrailColor(FColor::MakeRandomColor()); TrailFX->SetFloatParameter("JitterAmount", 0); BoidParams = BoidParamsLeader; return EBoidModificator::leader;  }()
		: 
		[&]()->EBoidModificator { SetTrailColor(BoidParams->DefaultTrailColor); TrailFX->SetFloatParameter("JitterAmount", 5); BoidParams = BoidParamsSbire;  return EBoidModificator::sbire;  }();
	
}

void ABoid::DrawDebug()
{
	if (BoidRole == EBoidModificator::leader)
		DrawDebugSphere(GetWorld(), GetActorLocation(), 10, 15, FColor::Blue);

	UE_LOG(LogTemp, Warning, TEXT("Flock num : %d"), Flock.Num());

}

bool ABoid::CheckForObstacle()
{

	if (BoidParams->GetAvoidanceSensors().Num() > 0)
	{
		FQuat SensorRotation = FQuat::FindBetweenVectors(BoidParams->GetAvoidanceSensors()[0], this->GetActorForwardVector());
		FVector NewSensorDirection = FVector::ZeroVector;
		NewSensorDirection = SensorRotation.RotateVector(BoidParams->GetAvoidanceSensors()[0]);
		FCollisionQueryParams TraceParameters;
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, this->GetActorLocation(), this->GetActorLocation() + NewSensorDirection * BoidParams->GetSensorRadius(), COLLISION_AVOIDANCE, TraceParameters);

		if (Hit.bBlockingHit)
		{
			TArray<AActor*> OverlapActors;
			SphereCollider->GetOverlappingActors(OverlapActors);
			for (AActor* OverlapActor : OverlapActors)
			{
				if (Hit.GetActor() == OverlapActor)
				{
					return false;
				}
			}
		}

		return Hit.bBlockingHit;

	}
	return false;
}

FVector ABoid::AvoidObstacle()
{
	FVector Steering = FVector::ZeroVector;
	FQuat SensorRotation = FQuat::FindBetweenVectors(BoidParams->GetAvoidanceSensors()[0], this->GetActorForwardVector());
	FVector NewSensorDirection = FVector::ZeroVector;
	FCollisionQueryParams TraceParameters;
	FHitResult Hit;

	for (FVector AvoidanceSensor : BoidParams->GetAvoidanceSensors())
	{
		NewSensorDirection = SensorRotation.RotateVector(AvoidanceSensor);
		GetWorld()->LineTraceSingleByChannel(Hit, this->GetActorLocation(), this->GetActorLocation() + NewSensorDirection * BoidParams->GetSensorRadius(), COLLISION_AVOIDANCE, TraceParameters);
		if (!Hit.bBlockingHit)
		{
			Steering = NewSensorDirection.GetSafeNormal() - Velocity.GetSafeNormal();
			Steering *= BoidParams->AvoidanceStrength;
			return Steering;
		}
	}

	return FVector::ZeroVector;
}

void ABoid::AddNewActorInList(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 Index, bool bFromSweep, const FHitResult& SweepResult)
{
	ABoid* Boid = Cast<ABoid>(OtherActor);
	if (Boid)
	{
		Flock.Add(Boid);
		if (BoidRole == EBoidModificator::leader && Boid->BoidRole == EBoidModificator::sbire)
			Boid->SetTrailColor(TrailColor);
	}
		
}

void ABoid::RemoveActorInList(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 Index)
{
	ABoid* Boid = Cast<ABoid>(OtherActor);
	if (Boid)
		Flock.Remove(Boid);
}



void ABoid::SetTrailColor(const FColor& Color)
{
	TrailColor = Color;
	TrailFX->SetColorParameter("Color", Color);

}




