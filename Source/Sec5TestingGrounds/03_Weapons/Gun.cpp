// MBI Copyrights

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "03_Weapons/Projectile.h"
#include "01_Player/FirstPersonCharacter.h"


AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Gun->bCastDynamicShadow = false;
	Gun->CastShadow = false;
	SetRootComponent(Gun);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Gun);
	Muzzle->SetRelativeLocation(FVector(0.f, 59.f, 11.f));
}

void AGun::OnFire()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = Cast<APawn>(GetAttachParentActor())->GetControlRotation();
			const FVector SpawnLocation = Muzzle->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// TODO See if it's better to do it like it's done in the course: FireAnimation and PawnAnimMesh are Gun UPROPERTY EditDefaultsOnly. See how it is adapted to PatrollingGuard
	// IDEA Create a new component GunCarrier that must be added to all the Pawns that can carry a Gun. This component will handle these 2 properties.
	//      Gun specification: To have the Pawn able to carry the Gun, it must have GunCarrier component attached and configured.
	//      Then use GetAttachParentActor()->FindComponentByClass<GunCarrier>() to get all the properties.
	// try and play a firing animation if specified
	UAnimMontage* FireAnimation = Cast<AFirstPersonCharacter>(GetAttachParentActor())->FireAnimation;
	if (FireAnimation != NULL)
	{
		// Get the animation object
		USkeletalMeshComponent* PawnAnimMesh = Cast<USkeletalMeshComponent>(GetAttachParentActor()->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), FName("PawnAnimatedMesh"))[0]);
		UAnimInstance* AnimInstance = PawnAnimMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
