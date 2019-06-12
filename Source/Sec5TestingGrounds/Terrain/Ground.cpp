// MBI Copyrights

#include "Ground.h"
#include "ActorPool.h"
#include "Engine/World.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "AI/Navigation/NavigationSystem.h"


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

	if (!ensure(NavMeshBoundsVolumePoolRef)) return;
	if (!ensure(NavMeshBoundsVolume)) return;
	NavMeshBoundsVolumePoolRef->ReturnToPool(NavMeshBoundsVolume);
	NavMeshBoundsVolume = nullptr;
}

/**
* Places X specified Actors, where X is a random number between MinSpawn and MaxSpawn.
* The Actors will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
* The Actor can be scaled X times where X is a random float between MinScale and MaxScale. The NeededSpaceRadius will also be scaled with the actor.
*/
void AGround::PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn, int32 MaxSpawn, int32 MaxAttempts, float NeededSpaceRadius, float MinScale, float MaxScale)
{
	if (!ensure(ActorToSpawn)) return;

	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition = FSpawnPosition(MinScale, MaxScale);
		if (FindEmptyLocation(SpawnPosition.Location, NeededSpaceRadius * SpawnPosition.Scale, MaxAttempts)) PlaceActor(ActorToSpawn, SpawnPosition);
	}
}

/**
* Places X specified Pawns, where X is a random number between MinSpawn and MaxSpawn.
* The Pawns will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
*/
void AGround::PlaceAIPawns(TSubclassOf<APawn> PawnToSpawn, int32 MinSpawn, int32 MaxSpawn, int32 MaxAttempts, float NeededSpaceRadius)
{
	if (!ensure(PawnToSpawn)) return;

	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition = FSpawnPosition();
		if (FindEmptyLocation(SpawnPosition.Location, NeededSpaceRadius * SpawnPosition.Scale, MaxAttempts))
		{
			// The BP_Character has an offset of 100cm on Z axis
			SpawnPosition.Location = SpawnPosition.Location + FVector(0, 0, 100);

			APawn* PlacedPawn = Cast<APawn>(PlaceActor(PawnToSpawn, SpawnPosition));
			
			if (!ensure(PlacedPawn)) return;
			PlacedPawn->SpawnDefaultController();
		}
	}
}

/**
* Generate grass on the Ground with the specified grass Hierarchical Instanced Static Mesh.
* The Ground will be sliced in square tiles with specified dimension.
* Each tile will be populated with X grass instances, where X is a random number between MinInstancesPerTile and MaxInstancesPerTile.
*/
void AGround::GenerateGrass(UHierarchicalInstancedStaticMeshComponent* GrassHISMC, float TileDimension, int32 MinInstancesPerTile, int32 MaxInstancesPerTile)
{
	if (!ensure(GrassHISMC)) return;

	TArray<FBox> Tiles = SliceGroundInTiles(TileDimension);
	for (FBox Tile : Tiles)
	{
		int32 GrassInstances = FMath::RandRange(MinInstancesPerTile, MaxInstancesPerTile);
		GenerateGrassOnTile(GrassHISMC, Tile, GrassInstances);
	}
}

/**
* Checks out a NavMeshBoundsVolume from the Game Mode pool then configure it to use in this Ground.
*/
void AGround::UseNavMeshBoundsVolumeFromPool(UActorPool * NavMeshBoundsVolumePoolToUse)
{
	if (!ensure(NavMeshBoundsVolumePoolToUse)) return;

	NavMeshBoundsVolumePoolRef = NavMeshBoundsVolumePoolToUse;
	NavMeshBoundsVolume = NavMeshBoundsVolumePoolRef->Checkout();

	PositionNavMeshBoundsVolume();

	GetWorld()->GetNavigationSystem()->Build();
}

/** 
* -------------------------
* -------------------------
* Utilities for PlaceActors
* -------------------------
* -------------------------
*/
/**
* Places the specified scaled Actor at the specified SpawnPoint with a randomly generated rotation.
*/
AActor* AGround::PlaceActor(TSubclassOf<AActor> ActorToSpawn, const FSpawnPosition& SpawnPosition)
{
	if (!ensure(ActorToSpawn)) return nullptr;
	// Adding a temporary empty location here, else the Actor will be spawned at (0, 0, 0) where an object has potentially been placed
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, FVector(0, 0, 5000), FRotator(0));

	if (!ensure(SpawnedActor)) return nullptr;
	SpawnedActor->SetActorLocation(SpawnPosition.Location);
	SpawnedActor->SetActorRotation(SpawnPosition.Rotation);
	SpawnedActor->SetActorRelativeScale3D(FVector(SpawnPosition.Scale));
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), NAME_None);
	return SpawnedActor;
}

bool AGround::FindEmptyLocation(FVector& OutLocation, float NeededSpaceRadius, int32 MaxAttempts)
{
	// Define the surface on which Actors have to Spawn. The surface is defined thanks to 2 points (Ground Floor Endpoints). These points are relative to the Ground Actor.
	FBox SpawnableSurface = FBox(MinExtent, MaxExtent);

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
* ---------------------------
* ---------------------------
* Utilities for GenerateGrass
* ---------------------------
* ---------------------------
*/
TArray<FBox> AGround::SliceGroundInTiles(float TileDimension)
{
	TArray<FBox> Tiles;

	for (float ProgressionOnX = MinExtent.X; ProgressionOnX < MaxExtent.X; ProgressionOnX = ProgressionOnX + TileDimension)
	{
		for (float ProgressionOnY = MinExtent.Y; ProgressionOnY < MaxExtent.Y; ProgressionOnY = ProgressionOnY + TileDimension)
		{
			FVector LeftBottom = FVector(ProgressionOnX, ProgressionOnY, MinExtent.Z);
			FVector RightTop = FVector(ProgressionOnX + TileDimension, ProgressionOnY + TileDimension, MinExtent.Z);
			FBox Tile = FBox(LeftBottom, RightTop);
			Tiles.Add(Tile);
		}
	}

	return Tiles;
}

void AGround::GenerateGrassOnTile(UHierarchicalInstancedStaticMeshComponent* GrassHISMC, FBox Tile, int32 Instances)
{
	if (!ensure(GrassHISMC)) return;
	for (int32 i = 0; i < Instances; i++)
	{
		FTransform SpawnTransform = FTransform(FRotator(0, FMath::FRandRange(-180, 180), 0), FMath::RandPointInBox(Tile));
		GrassHISMC->AddInstance(SpawnTransform);
	}
}

/**
* --------------------------------------------
* --------------------------------------------
* Utilities for UseNavMeshBoundsVolumeFromPool
* --------------------------------------------
* --------------------------------------------
*/
void AGround::PositionNavMeshBoundsVolume()
{
	if (!ensure(NavMeshBoundsVolume)) return;
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + FVector((MinExtent.X + MaxExtent.X) / 2, (MinExtent.Y + MaxExtent.Y) / 2, (MinExtent.Z + MaxExtent.Z) / 2));
}
