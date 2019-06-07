// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API AInfiniteTerrainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void PopulateNavVolumePool();

private:
	void AddToPool(class ANavMeshBoundsVolume* NavVolumeToAdd);
};
