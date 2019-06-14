// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ground.generated.h"

USTRUCT()
struct FSpawnParameters
{
	GENERATED_USTRUCT_BODY()

	FSpawnParameters()
	{
		Location = FVector(0);
		Rotation = FRotator(0, FMath::FRandRange(-180, 180), 0);
		Scale = 1;
	};

	FSpawnParameters(float MinScale, float MaxScale)
	{
		Location = FVector(0);
		Rotation = FRotator(0, FMath::FRandRange(-180, 180), 0);
		Scale = FMath::FRandRange(MinScale, MaxScale);
	};

	FVector Location;
	FRotator Rotation;
	float Scale;
};


UCLASS()
class SEC5TESTINGGROUNDS_API AGround : public AActor
{
	GENERATED_BODY()
	
public:	
	AGround();

	/** Used to return NavMeshBoundsVolume to the NavMeshBoundsVolumePoolRef */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	* Places X specified Actors, where X is a random number between MinSpawn and MaxSpawn.
	* The Actors will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
	* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
	* The Actor can be scaled X times where X is a random float between MinScale and MaxScale. The NeededSpaceRadius will also be scaled with the actor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, int32 MaxAttempts = 20, float NeededSpaceRadius = 500, float MinScale = 1, float MaxScale = 1);

	/**
	* Places X specified Pawns, where X is a random number between MinSpawn and MaxSpawn.
	* The Pawns will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
	* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void PlaceAIPawns(TSubclassOf<APawn> PawnToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, int32 MaxAttempts = 20, float NeededSpaceRadius = 500);

	/**
	* Generate grass on the Ground with the specified grass Hierarchical Instanced Static Mesh.
	* The Ground will be sliced in square tiles with specified dimension.
	* Each tile will be populated with X grass instances, where X is a random number between MinInstancesPerTile and MaxInstancesPerTile.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void GenerateGrass(class UHierarchicalInstancedStaticMeshComponent* GrassHISMC, float TileDimension = 200, int32 MinInstancesPerTile = 3, int32 MaxInstancesPerTile = 5);

	/**
	* Checks out a NavMeshBoundsVolume from the Game Mode pool then configure it to use in this Ground.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void UseNavMeshBoundsVolumeFromPool(class UActorPool* NavMeshBoundsVolumePoolToUse);

protected:
	/** Ground Dimensions. Must be set from the Editor if the Ground dimensions and location change. */
	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	FVector MinExtent = FVector(0, -2000, 0);
	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	FVector MaxExtent = FVector(4000, 2000, 0);

private:
	/** Utilities for PlaceActors */
	AActor* PlaceActor(TSubclassOf<AActor> ActorToSpawn, const FSpawnParameters& SpawnPosition);
	bool FindEmptyLocation(FVector& OutLocation, float NeededSpaceRadius, int32 MaxAttempts);
	bool IsEmpty(FVector RelativeLocation, float Radius);

	/** Utilities for GenerateGrass */
	TArray<FBox> SliceGroundInTiles(float TileDimension);
	void GenerateGrassOnTile(class UHierarchicalInstancedStaticMeshComponent* GrassHISMC, FBox Tile, int32 Instances);

	/** Utilities for UseNavMeshBoundsVolumeFromPool */
	class UActorPool* NavMeshBoundsVolumePoolRef;
	AActor* NavMeshBoundsVolume;
	void PositionNavMeshBoundsVolume();
};
