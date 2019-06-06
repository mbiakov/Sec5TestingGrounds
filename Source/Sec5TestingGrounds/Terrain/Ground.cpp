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

	CastSphere(GetActorLocation(), 300);
	CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);
}

void AGround::PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn, int32 MaxSpawn)
{
	// Define the surface on which Actors have to Spawn. The surface is defined thanks to 2 points (Ground Floor Endpoints)
	FVector BottomRightEndpoint = FVector(0, 1950, 100);
	FVector TopLeftEndpoint = FVector(3950, -1950, 100);

	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(FBox(BottomRightEndpoint, TopLeftEndpoint));
		FRotator SpawnRotation = FRotator(0, FMath::FRandRange(0, 360), 0);
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPoint, SpawnRotation);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), NAME_None);
	}
}

bool AGround::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Camera,
		FCollisionShape::MakeSphere(Radius)
	);

	FLinearColor ResultColor = HasHit ? FLinearColor::Red : FLinearColor::Green;
	UKismetSystemLibrary::DrawDebugSphere(
		this,
		Location,
		Radius,
		36,
		ResultColor
	);

	return HasHit;
}
