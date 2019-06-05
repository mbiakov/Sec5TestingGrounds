// MBI Copyrights

#include "UnitedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
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

	// The default Mesh introduced by the ACharacter class is used as Third Person Mesh
}

void AUnitedCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Gun attachment is done here because in the constructor the skeleton is not yet created and the GunBlueprint is not yet set
	AttachGun();
	
}

void AUnitedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnitedCharacter::PullTrigger);
}

void AUnitedCharacter::PullTrigger()
{
	if (!Gun) return;
	Gun->OnFire();

	// TODO ReportNoise: use GetActorLocation() as Noise Location and Self as Instigator. In the Gun ?
}

void AUnitedCharacter::AttachGun()
{
	if (!ensure(GunBlueprint)) return;
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);

	if (IsPlayerControlled()) {
		// Attaching to the First Person Skeletal Mesh and setting the First Person Fire animation
		Gun->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		// Configuration: only owner see
		Gun->SetOwner(this);
		Gun->GetGunMesh()->SetOnlyOwnerSee(true);
		Gun->GetGunMesh()->bCastDynamicShadow = false;
		Gun->GetGunMesh()->CastShadow = false;
		// Configuration: firing animations
		if (!ensure(FirstPersonMesh->GetAnimInstance() && FirstPersonFireAnimation)) return;
		Gun->SetAnimInstance(FirstPersonMesh->GetAnimInstance());
		Gun->SetFireAnimation(FirstPersonFireAnimation);
	}
	else { // Is AI Controlled
		// Attaching to the Third Person Skeletal Mesh (ACharacter::GetMesh() returns Mesh that is used as Third Person Skeletal Mesh) and setting the Third Person Fire animation
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		// Configuration: owner no see
		Gun->SetOwner(this);
		Gun->GetGunMesh()->SetOwnerNoSee(true);
		// Configuration: firing animations
		if (!ensure(GetMesh()->GetAnimInstance() && ThirdPersonFireAnimation)) return;
		Gun->SetAnimInstance(GetMesh()->GetAnimInstance());
		Gun->SetFireAnimation(ThirdPersonFireAnimation);
	}
}
