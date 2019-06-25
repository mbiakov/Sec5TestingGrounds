// MBI Copyrights

#pragma once

#include "Perception/AIPerceptionTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAIController.generated.h"

/**
 * Guard Behavior State used to determine the Bahavior
 */
UENUM()
enum EGuardBahaviorState
{
	NoOngoingAction,
	MovingToTheNextWaypoint,
	Waiting,
};

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGuardAIController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Perceprion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

	// EQS
	UPROPERTY(EditDefaultsOnly, Category = "EQS")
	class UEnvQuery* EQSQuery;

	// Behavior
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MinWaitTime = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float MaxWaitTime = 4;

private:
	// Behavior
	EGuardBahaviorState ActualGuardBehavior = EGuardBahaviorState::NoOngoingAction;
	float MovementStartedAt = 0;
	float ActualWaitTime = 0;
	float WaitStartedAt = 0;

	// Perceprion
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// EQS
	void MoveToNextWaypoint(TSharedPtr<FEnvQueryResult> Result);
	FEnvQueryRequest FindNextWaypointEQSRequest;
	FVector NextWaypoint = FVector(0);
};
