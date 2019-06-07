// MBI Copyrights

#include "InfiniteTerrainGameMode.h"
#include "ActorPool.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
	: Super()										// Calling Super constructor
{
	/** Configure the game mode in the constructor if needed. Examples: */
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	HUDClass = ASec5TestingGroundsHUD::StaticClass();*/

	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("NavMeshBoundsVolumePool"));
}

void AInfiniteTerrainGameMode::PopulateNavMeshBoundsVolumePool()
{
	TActorIterator<ANavMeshBoundsVolume> NavVolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (NavVolumeIterator)
	{
		NavMeshBoundsVolumePool->AddToPool(*NavVolumeIterator);
		++NavVolumeIterator;
	}
}
