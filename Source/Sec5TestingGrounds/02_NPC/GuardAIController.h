// MBI Copyrights

#pragma once

#include "Perception/AIPerceptionTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Timer.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAIController.generated.h"

/**
 * Guard Behavior State used to determine the Bahavior
 */
UENUM()
enum EGuardBahaviorState
{
	EnemyDetected,
	Suspicious,
	WaitingOnPatrolPoint,
	MovingToTheNextPatrolPoint,
};

/**
 * The controlled Pawn must be of class AUnitedCharacter to manage MustAim variable used in AUnitedCharacter Animations.
 */
UCLASS()
class SEC5TESTINGGROUNDS_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGuardAIController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	/** Perceprion */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

	/** Behavior */
	/** Wait Time */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MinPatrolPointWaitTime = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MaxPatrolPointWaitTime = 4;
	/** Shooting */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MinTimeBetweenTwoBursts = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MaxTimeBetweenTwoBursts = 4;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	int MinShootsInBurst = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	int MaxShootsInBurst = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float ShootFrequencyInBurst = 0.15;
	/** EQS (for Behavior) */
	UPROPERTY(EditDefaultsOnly, Category = "EQS")
	class UEnvQuery* EQSQuery;

private:
	/** Behavior */
	void PerformStateTransitions();
	void ExecuteStateActions();

	/** Behavior Utilities */
	bool NoMoreMovement();
	class AUnitedCharacter* ControlledCharacter;
	FTimerUtility Timer = FTimerUtility();
	EGuardBahaviorState ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
	AActor* DetectedEnemy;
	bool EnemyDetected = false; // For EnemyDetected State Transitions
	/** Shooting */
	void ShootAtEnemy();
	bool FindGun();
	class AGun* Gun;
	bool MustShootABurst = false;
	int32 ShootsInBurst = 0;
	int32 ActualShootInBurst = 0;
	/** EQS (for Behavior) */
	void MoveToNextPatrolPointOnEQSExecuted(TSharedPtr<FEnvQueryResult> Result);
	FEnvQueryRequest FindNextPatrolPointEQSRequest;
	FVector NextPatrolPoint = FVector(0);

	/** Perceprion */
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
