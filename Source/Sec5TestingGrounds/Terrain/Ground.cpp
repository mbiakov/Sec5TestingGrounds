// MBI Copyrights

#include "Ground.h"


AGround::AGround()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGround::BeginPlay()
{
	Super::BeginPlay();

	PlaceActors();
}

void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGround::PlaceActors()
{
	// FMath::RandRange();

	for (int32 i = 0; i < 5; i++)
	{
		FVector RandomVector = FMath::RandPointInBox(FBox(GetActorLocation() + FVector(0, 1950, 100), GetActorLocation() + FVector(3950, -1950, 100)));
		UE_LOG(LogTemp, Warning, TEXT("Ground %s random point: %s"), *GetName(), *RandomVector.ToString());
	}
}
