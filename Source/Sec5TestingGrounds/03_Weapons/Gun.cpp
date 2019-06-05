// MBI Copyrights

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "03_Weapons/Projectile.h"


AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	SetRootComponent(Gun);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Gun);
	Muzzle->SetRelativeLocation(FVector(0.f, 65.f, 11.f));
}

void AGun::OnFire()
{
	if (ProjectileClass)
	{
		const FRotator SpawnRotation = Cast<APawn>(GetAttachParentActor())->GetControlRotation();
		const FVector SpawnLocation = Muzzle->GetComponentLocation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (FireAnimation && AnimInstance)
	{
		AnimInstance->Montage_Play(FireAnimation, 1.f);
	}
}
