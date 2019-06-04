// MBI Copyrights

#include "UnitedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "03_Weapons/Gun.h"


AUnitedCharacter::AUnitedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FirstPersonMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

void AUnitedCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Attach First Person Gun to the Skeleton, doing it here because the skeleton is not yet created in the constructor and the GunBlueprint not yet set
	if (!ensure(GunBlueprint)) return;
	FirstPersonGun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	FirstPersonGun->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FirstPersonGun->SetOwner(this);
	FirstPersonGun->GetGunMesh()->SetOnlyOwnerSee(true);
	FirstPersonGun->GetGunMesh()->bCastDynamicShadow = false;
	FirstPersonGun->GetGunMesh()->CastShadow = false;
}

void AUnitedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnitedCharacter::OnFire()
{
	// TODO The firing is different regarding if it is the Player or the AI. Use something like: if (GetPlayerPawn() == self).

	if (!FirstPersonGun) return;

	// TODO ReportNoise: use GetActorLocation() as Noise Location and Self as Instigator
	// TODO In Gun ?

	FirstPersonGun->OnFire();

	// Play animation
	if (FireAnimation)
	{
		UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
