// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitedCharacter.generated.h"

UCLASS()
class SEC5TESTINGGROUNDS_API AUnitedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** The default Mesh introduced by the ACharacter class is used as Third Person Mesh */

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** First person Gun */
	class AGun* Gun;

public:
	AUnitedCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	/** When possessed by a Player, deteches the Gun from the First Person Mesh and attach it to the Third Person Mesh */
	virtual void UnPossessed() override;

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category = Setup)
	void PullTrigger();

	/** Section: First Person Gun */
	/** Gun Class to Spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Shooting Configuration")
	TSubclassOf<class AGun> GunBlueprint;
	/** First Person Gun Fire Animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Configuration")
	class UAnimMontage* FirstPersonFireAnimation;
	/** Third Person Gun Fire Animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting Configuration")
	class UAnimMontage* ThirdPersonFireAnimation;

	/** Section: Animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	bool MustAim;

private:
	void AttachGun();
};
