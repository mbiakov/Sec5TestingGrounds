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

	//Attach First Person Gun to the Skeleton, doing it here because the skeleton is not yet created in the constructor and the GunBlueprint not yet set
	if (!ensure(GunBlueprint)) return;
	FirstPersonGun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	FirstPersonGun->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	// Configuration: only owner see
	FirstPersonGun->SetOwner(this);
	FirstPersonGun->GetGunMesh()->SetOnlyOwnerSee(true);
	FirstPersonGun->GetGunMesh()->bCastDynamicShadow = false;
	FirstPersonGun->GetGunMesh()->CastShadow = false;
	// Configuration: firing animations
	if (!ensure(FirstPersonMesh->GetAnimInstance() && FirstPersonFireAnimation)) return;
	FirstPersonGun->SetAnimInstance(FirstPersonMesh->GetAnimInstance());
	FirstPersonGun->SetFireAnimation(FirstPersonFireAnimation);

	//Attach Third Person Gun to the Skeleton, doing it here because the skeleton is not yet created in the constructor and the GunBlueprint not yet setif (!ensure(GunBlueprint)) return;
	ThirdPersonGun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	ThirdPersonGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); // ACharacter::GetMesh() returns Mesh that is used as Third Person Skeletal Mesh
	// Configuration: owner no see
	ThirdPersonGun->SetOwner(this);
	ThirdPersonGun->GetGunMesh()->SetOwnerNoSee(true);
	// Configuration: firing animations
	if (!ensure(GetMesh()->GetAnimInstance() && ThirdPersonFireAnimation)) return; // ACharacter::GetMesh() returns Mesh that is used as Third Person Skeletal Mesh
	ThirdPersonGun->SetAnimInstance(GetMesh()->GetAnimInstance());
	ThirdPersonGun->SetFireAnimation(ThirdPersonFireAnimation);
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
	if (!FirstPersonGun || !ThirdPersonGun) return;

	// Depending on whether this is the Player Pawn or AI Pawn we fire respectively FirstPersonGun or ThirdPersonGun
	if (this == UGameplayStatics::GetPlayerPawn(this, 0)) {
		FirstPersonGun->OnFire();
	}
	else {
		ThirdPersonGun->OnFire();
	}

	// TODO ReportNoise: use GetActorLocation() as Noise Location and Self as Instigator. In the Gun ?
}
