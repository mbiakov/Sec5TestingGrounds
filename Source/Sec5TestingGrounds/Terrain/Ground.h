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
	* Places X Actors, where X is a random number between MinSpawn and MaxSpawn.
	* The Actors will be placed only if an empty location is found. The MaxAttempts parameter specifies the number of attempts to find an empty location.
	* The NeededSpaceRadius specifies the radius within which we will look for collisions. Must be the space needed by the Actor.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, int32 MaxAttempts = 20, float NeededSpaceRadius = 500);

private:
	/** Utilities for PlaceActors */
	void PlaceActor(TSubclassOf<AActor> ActorToSpawn, FVector SpawnPoint);
	bool FindEmptyLocation(FVector& OutLocation, float NeededSpaceRadius, int32 MaxAttempts);
	bool IsEmpty(FVector RelativeLocation, float Radius);
};
