// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

/**
* Requirements:
*   - The Pawn on witch the Gun is attached must have the tag "PawnAnimatedMesh" on its animated Skeletal Mesh Component. Needed to create firing animation. See OnFire() method.
*	- The Pawn on witch the Gun is attached must have UAnimMontage* FireAnimation property. Needed to create firing animation. See OnFire() method.
*/
UCLASS()
class SEC5TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()

	/** Gun mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* Muzzle;
	
public:	
	AGun();

	/** Fires a projectile. */
	void OnFire();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Gun Configuration")
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Configuration")
	class USoundBase* FireSound;
};
