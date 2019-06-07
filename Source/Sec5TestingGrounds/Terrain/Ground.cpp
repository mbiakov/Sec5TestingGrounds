// MBI Copyrights

#include "Ground.h"
#include "ActorPool.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"


AGround::AGround()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGround::BeginPlay()
{
	Super::BeginPlay();
}

void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGround::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!NavMeshBoundsVolumePoolRef || !NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s AGround::EndPlay() NavMeshBoundsVolumePoolRef or NavMeshBoundsVolume not found"), *this->GetName());
		return;
	}
	NavMeshBoundsVolumePoolRef->ReturnToPool(NavMeshBoundsVolume);
	NavMeshBoundsVolume = nullptr;
}

/**
* Generate grass on the Ground with the specified grass Hierarchical Instanced Static Mesh.
* The Ground will be sliced in square tiles with specified dimension.
* Each tile will be populated with X grass instances, where X is a random number between MinInstancesPerTile and MaxInstancesPerTile.
*/
void AGround::GenerateGrass(UHierarchicalInstancedStaticMeshComponent* GrassHISMC, float TileDimension, int32 MinInstancesPerTile, int32 MaxInstancesPerTile)
{
	TArray<FBox> Tiles = SliceGroundInTiles(TileDimension);
	for (FBox Tile : Tiles)
	{
		int32 GrassInstances = FMath::RandRange(MinInstancesPerTile, MaxInstancesPerTile);
		GenerateGrassOnTile(GrassHISMC, Tile, GrassInstances);
	}
}

TArray<FBox> AGround::SliceGroundInTiles(float TileDimension)
{
	TArray<FBox> Tiles;

	for (float ProgressionOnX = 0; ProgressionOnX < 4000; ProgressionOnX = ProgressionOnX + TileDimension)
	{
		for (float ProgressionOnY = -1950; ProgressionOnY < 1950; ProgressionOnY = ProgressionOnY + TileDimension)
		{
			FVector LeftBottom = FVector(ProgressionOnX, ProgressionOnY, 0);
			FVector RightTop = FVector(ProgressionOnX + TileDimension, ProgressionOnY + TileDimension, 0);
			FBox Tile = FBox(LeftBottom, RightTop);
			Tiles.Add(Tile);
		}
	}

	return Tiles;
}

void AGround::GenerateGrassOnTile(UHierarchicalInstancedStaticMeshComponent* GrassHISMC, FBox Tile, int32 Instances)
{
	for (int32 i = 0; i < Instances; i++)
	{
		FTransform SpawnTransform = FTransform(FRotator(0, FMath::FRandRange(-180, 180), 0), FMath::RandPointInBox(Tile));
		GrassHISMC->AddInstance(SpawnTransform);
	}
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
	FRotator RandomSpawnRotation = FRotator(0, FMath::FRandRange(-180, 180), 0);
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

/**
* Checks out a NavMeshBoundsVolume from the Game Mode pool then configure it to use in this Ground.
*/
void AGround::UseNavMeshBoundsVolumeFromPool(UActorPool * NavMeshBoundsVolumePoolToUse)
{
	NavMeshBoundsVolumePoolRef = NavMeshBoundsVolumePoolToUse;

	PositionNavMeshBoundsVolume();
}

void AGround::PositionNavMeshBoundsVolume()
{
	if (!NavMeshBoundsVolumePoolRef)
	{
		UE_LOG(LogTemp, Error, TEXT("%s AGround::PositionNavMeshBoundsVolume() NavMeshBoundsVolumePoolRef isn't set"), *this->GetName());
		return;
	}
	NavMeshBoundsVolume = NavMeshBoundsVolumePoolRef->Checkout();

	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s AGround::PositionNavMeshBoundsVolume() unable to get a NavMesh from Pool"), *this->GetName());
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + FVector(2000, 0, 0));
	// TODO Force navigation recalculation
}
