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
	MovingToTheNextPatrolPoint,
	WaitingOnPatrolPoint,
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

	// Perceprion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

	// Behavior
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MinPatrolPointWaitTime = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MaxPatrolPointWaitTime = 4;
	// EQS (for Behavior)
	UPROPERTY(EditDefaultsOnly, Category = "EQS")
	class UEnvQuery* EQSQuery;

private:
	// Behavior
	void ShootAtEnemy();
	bool NoMoreMovement();
	class AUnitedCharacter* ControlledCharacter;
	FTimerUtility Timer = FTimerUtility();
	EGuardBahaviorState ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
	AActor* DetectedEnemy;
	// EQS (for Behavior)
	void MoveToNextPatrolPointOnEQSExecuted(TSharedPtr<FEnvQueryResult> Result);
	FEnvQueryRequest FindNextPatrolPointEQSRequest;
	FVector NextPatrolPoint = FVector(0);

	// Perceprion
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
