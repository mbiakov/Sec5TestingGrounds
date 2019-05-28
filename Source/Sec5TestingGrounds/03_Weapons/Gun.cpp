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

	// TODO Rename to Gun
	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	// TODO Rename to Muzzle
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// TODO Verify this works once the Gun is attached
			// Course version: const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
			const FRotator SpawnRotation = Cast<APawn>(GetAttachParentActor())->GetControlRotation();

			const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.2f);
	}

	// try and play a firing animation if specified
	// TODO Verify this works once attached
	// TOOD Make it more general than just FirstPersonCharacter
	// In the course it as a Edit Uprop
	UAnimMontage* FireAnimation = Cast<AFirstPersonCharacter>(GetAttachParentActor())->FireAnimation;
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		// TODO Verify this woks once attached to the Pawn
		// TODO Delete the commented code
		// Initial version: UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		// In the course it's an edit uproperty
		UAnimInstance* AnimInstance = GetAttachParentActor()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
