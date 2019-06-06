// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ground.generated.h"

UCLASS()
class SEC5TESTINGGROUNDS_API AGround : public AActor
{
	GENERATED_BODY()
	
public:	
	AGround();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/**
	* Generate grass on the Ground with the specified grass Hierarchical Instanced Static Mesh.
	* The Ground will be sliced in square tiles with specified dimension.
	* Each tile will be populated with X grass instances, where X is a random number between MinInstancesPerTile and MaxInstancesPerTile.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void GenerateGrass(class UHierarchicalInstancedStaticMeshComponent* GrassHISMC, float TileDimension = 200, int32 MinInstancesPerTile = 3, int32 MaxInstancesPerTile = 5);

	/**
	* Places X specified Actors, where X is a random number between MinSpawn and MaxSpawn.
	* The Actors will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
	* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
	* The Actor can be scaled X times where X is a random float between MinScale and MaxScale. The NeededSpaceRadius will also be scaled with the actor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, int32 MaxAttempts = 20, float NeededSpaceRadius = 500, float MinScale = 1, float MaxScale = 1);

private:
	/** Utilities for GenerateGrass */
	TArray<FBox> SliceGroundInTiles(float TileDimension);
	void GenerateGrassOnTile(class UHierarchicalInstancedStaticMeshComponent* GrassHISMC, FBox Tile, int32 Instances);

	/** Utilities for PlaceActors */
	void PlaceActor(TSubclassOf<AActor> ActorToSpawn, FVector SpawnPoint, float Scale);
	bool FindEmptyLocation(FVector& OutLocation, float NeededSpaceRadius, int32 MaxAttempts);
	bool IsEmpty(FVector RelativeLocation, float Radius);
};
