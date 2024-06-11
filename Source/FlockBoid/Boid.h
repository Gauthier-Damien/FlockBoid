// Flock Boid, Craig Reynolds, 1986 
// Source : https://fr.wikipedia.org/wiki/Boids / https://medium.com/fragmentblog/simulating-flocking-with-the-boids-algorithm-92aef51b9e00 / https://www.youtube.com/@turingcode

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidDataAsset.h"
#include "NiagaraComponent.h"
#include <Components/SphereComponent.h>
#include "Boid.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeaderChange, AActor*, otherActor);



UENUM()
enum EBoidModificator
{
	leader,
	sbire,
};


class ABoidManager;

UCLASS()
class FLOCKBOID_API ABoid : public AActor
{
	GENERATED_BODY()

#pragma region Property
protected:
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Manager)
		TObjectPtr<USphereComponent> SphereCollider = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Role)
		TEnumAsByte<EBoidModificator> BoidRole = EBoidModificator::sbire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
		TObjectPtr<UBoidDataAsset> BoidParamsSbire = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameters)
		TObjectPtr<UBoidDataAsset> BoidParamsLeader = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		TObjectPtr<UNiagaraSystem> LeaderFX;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		TObjectPtr<UNiagaraSystem> SbireFX;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
		bool bAlreadyCheck = false;

private:
	TObjectPtr<AActor> Leader = NULL;
	TObjectPtr<UNiagaraComponent> TrailFX = NULL;
	TObjectPtr<UBoidDataAsset> BoidParams = NULL;
	FTimerHandle Timer;
	TArray<ABoid*> Flock;
	FColor TrailColor;
#pragma endregion Property



#pragma region Constructor
public:	
	ABoid();

#pragma endregion Constructor


#pragma region Methods
private:
	// Leader Test, Work In Progress
	void CheckLeadership();
	/// <summary>
	/// Debug Boid
	/// </summary>
	void DrawDebug();
	bool CheckForObstacle();
	/// <summary>
	/// Avoid Obstacles Work In Progress
	/// </summary>
	/// <returns></returns>
	FVector AvoidObstacle();
	UFUNCTION()
	void AddNewActorInList(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 Index, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RemoveActorInList(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 Index);
protected:
	virtual void BeginPlay() override;
	void InitColliderSphere();
	/// <summary>
	/// Default Behavior Methods, apply Separation, Alignment and Coherence
	/// </summary>
	/// <param name="_dt"></param>
	void Steer(float DeltaTime);
	void StayInBounds();
	/// <summary>
	/// Calculate Separation Force
	/// Separation => Force that change direction of Boid to close of another
	/// </summary>
	/// <param name="_boids"></param>
	/// <returns></returns>
	FVector Separate(TArray<ABoid*> Boids);
	/// <summary>
	/// Calculate Alignment Force
	/// Alignment => Define if bird follow the same direction as FLock
	/// </summary>
	/// <param name="Boids"></param>
	/// <returns></returns>
	FVector Alignment(TArray<ABoid*> Boids);
	/// <summary>
	/// Calculate Coherence Force
	/// Coherence => grouping
	/// </summary>
	/// <param name="Boids"></param>
	/// <returns></returns>
	FVector Coherence(TArray<ABoid*> Boids);
public:	
	/// <summary>
	/// debug Color, for visibility
	/// </summary>
	/// <param name="Color"></param>
	void SetTrailColor(const FColor& Color);
	virtual void Tick(float DeltaTime) override;
	// Leader Test, Work In Progress
	void SetAsLeader();
#pragma endregion Methods

};
