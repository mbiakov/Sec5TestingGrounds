// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

/**
* 
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

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void OnFire();

	class USkeletalMeshComponent* GetGunMesh() { return Gun; }

	/** Section: Fire Animation */
	void SetAnimInstance(class UAnimInstance* AnimInstanceToSet) { AnimInstance = AnimInstanceToSet; }
	void SetFireAnimation(class UAnimMontage* FireAnimationToSet) { FireAnimation = FireAnimationToSet; }

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Gun Configuration")
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Configuration")
	class USoundBase* FireSound;

private:
	/** Section: Fire Animation */
	class UAnimInstance* AnimInstance;
	class UAnimMontage* FireAnimation;
};
