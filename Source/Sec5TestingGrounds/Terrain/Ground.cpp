// MBI Copyrights

#include "Ground.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"


AGround::AGround()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGround::BeginPlay()
{
	Super::BeginPlay();
}

void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
* Places X specified Actors, where X is a random number between MinSpawn and MaxSpawn.
* The Actors will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
* The Actor can be scaled X times where X is a random float between MinScale and MaxScale. The NeededSpaceRadius will also be scaled with the actor.
*/
void AGround::PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn, int32 MaxSpawn, int32 MaxAttempts, float NeededSpaceRadius, float MinScale, float MaxScale)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		float Scale = FMath::FRandRange(MinScale, MaxScale);
		FVector SpawnPoint(0);
		if (FindEmptyLocation(SpawnPoint, NeededSpaceRadius * Scale, MaxAttempts)) PlaceActor(ActorToSpawn, SpawnPoint, Scale);
	}
}

/**
* Places the specified scaled Actor at the specified SpawnPoint with a randomly generated rotation. 
*/
void AGround::PlaceActor(TSubclassOf<AActor> ActorToSpawn, FVector SpawnPoint, float Scale)
{
	FRotator RandomSpawnRotation = FRotator(0, FMath::FRandRange(0, 360), 0);
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPoint, RandomSpawnRotation);
	SpawnedActor->SetActorRelativeScale3D(FVector(Scale));
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), NAME_None);
}

bool AGround::FindEmptyLocation(FVector& OutLocation, float NeededSpaceRadius, int32 MaxAttempts)
{
	// Define the surface on which Actors have to Spawn. The surface is defined thanks to 2 points (Ground Floor Endpoints). These points are relative to the Ground Actor.
	FVector FloorBottomRightEndpoint = FVector(0, 1950, 0);
	FVector FloorTopLeftEndpoint = FVector(3950, -1950, 0);
	FBox SpawnableSurface = FBox(FloorBottomRightEndpoint, FloorTopLeftEndpoint);

	for (int32 i = 0; i < MaxAttempts; i++)
	{
		FVector CandidateLocation = FMath::RandPointInBox(SpawnableSurface);
		if (IsEmpty(CandidateLocation, NeededSpaceRadius))
		{
			OutLocation = CandidateLocation;
			return true;
		}
	}

	return false;
}

bool AGround::IsEmpty(FVector RelativeLocation, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = GetActorLocation() + RelativeLocation;

	/** Sphere Cast */
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	/** The location is empty if any hit was found */
	return !HasHit;
}
