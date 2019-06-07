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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	class UActorPool* NavMeshPool;

private:
	void AddToPool(class ANavMeshBoundsVolume* NavVolumeToAdd);
};
