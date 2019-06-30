// MBI Copyrights

#include "GuardAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "Character/UnitedCharacter.h"

AGuardAIController::AGuardAIController()
{
	// Start Perception Configuration
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerceptionComponent"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight"));
	Sight->SightRadius = 800;
	Sight->LoseSightRadius = 1000;
	Sight->PeripheralVisionAngleDegrees = 70;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerceptionComponent->ConfigureSense(*Sight);

	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(FName("Hearing"));
	Hearing->HearingRange = 1000;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerceptionComponent->ConfigureSense(*Hearing);

	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardAIController::OnTargetPerceptionUpdated);
	// End Perception Configuration
}

void AGuardAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup EQS Request
	if (!ensure(EQSQuery)) return;
	FindNextPatrolPointEQSRequest = FEnvQueryRequest(EQSQuery, this);
}

void AGuardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Initialize
	ControlledCharacter = Cast<AUnitedCharacter>(InPawn);
}

void AGuardAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformStateTransitions();
	ExecuteStateActions();
}

void AGuardAIController::PerformStateTransitions()
{
	// For the following State Transition, if the Enemy is detected, we must enter the state EnemyDetected independently of the actual state.
	// We still must verify that we are not already in the State EnemyDetected, else the transition actions will be always executed.
	if (ActualGuardBehavior != EGuardBahaviorState::EnemyDetected && EnemyDetected) {
		PatrolPointWaitTime.Clear(); // Clear Timers from other States
		NoMoreMovementWaitTime.Clear(); // Clear Timers from other States
		MustShootABurst = true; // Must be set to true if we want the AI shoot at us as soon as he sees us
		ActualGuardBehavior = EGuardBahaviorState::EnemyDetected;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::EnemyDetected && !EnemyDetected) {
		// Move to the last know location.
		// Since we move to a fixe location the movement can be done in the transition for optimization.
		MoveToLocation(DetectedEnemy->GetActorLocation());
		ShootsInBurst = 0; // Reinitialize shoot variable
		ActualShootInBurst = 0; // Reinitialize shoot variable
		BetweenShootsWaitTime.Clear(); // Clear shoot Timers
		BetweenBurstsWaitTime.Clear(); // Clear shoot Timers
		ActualGuardBehavior = EGuardBahaviorState::Suspicious;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::Suspicious) {
		// There is a Timer in NoMoreMovement() so we want to be sure the condition NoMoreMovement() is not evaluated if the state is not Suspicious
		if (NoMoreMovement()) ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::MovingToTheNextPatrolPoint) {
		// There is a Timer in NoMoreMovement() so we want to be sure the condition NoMoreMovement() is not evaluated if the state is not Suspicious
		if (NoMoreMovement()) ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::WaitingOnPatrolPoint) {
		if (PatrolPointWaitTime.HasPassed()) {
			// Since we move to a fixe location the movement can be done in the transition for optimization.
			FindNextPatrolPointEQSRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &AGuardAIController::MoveToNextPatrolPointOnEQSExecuted);
			ActualGuardBehavior = EGuardBahaviorState::MovingToTheNextPatrolPoint;
		}
	}
}

void AGuardAIController::ExecuteStateActions()
{
	if (ActualGuardBehavior == EGuardBahaviorState::EnemyDetected) {
		MoveToActor(DetectedEnemy, 300);
		ShootAtEnemy();
	}
}

void AGuardAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor != UGameplayStatics::GetPlayerPawn(this, 0)) return;
	// From here we know that we detected the player

	if (Stimulus.WasSuccessfullySensed()) {
		DetectedEnemy = Actor;
		SetFocus(DetectedEnemy);
		MustAim = true;
		EnemyDetected = true;
	}
	if (!Stimulus.WasSuccessfullySensed()) {
		ClearFocus(EAIFocusPriority::Gameplay);
		MustAim = false;
		EnemyDetected = false;
	}
}

void AGuardAIController::ShootAtEnemy()
{
	if (!MustShootABurst) {
		if (BetweenBurstsWaitTime.HasPassed()) MustShootABurst = true;
	}

	if (MustShootABurst) {
		// Initialization for each burst
		if (ShootsInBurst == 0) ShootsInBurst = FMath::RandRange(MinShootsInBurst, MaxShootsInBurst);

		if (BetweenShootsWaitTime.HasPassed()) {
			ControlledCharacter->PullTrigger();

			ActualShootInBurst++;
			if (ActualShootInBurst >= ShootsInBurst) { // Burst finished, reinitialize shooting variables
				MustShootABurst = false;
				ShootsInBurst = 0;
				ActualShootInBurst = 0;
			}
		}
	}
}

bool AGuardAIController::NoMoreMovement()
{
	// We need to wait before checking for Velocity, because at the beginning of the movement the Velocity is 0.
	// If we don't wait before checking, the Guard State will go to Waiting directly at the Begging of the Movement.
	// Wait Time count will start, while moving. When the Guard will arrive to destination, he will not wait anymore because the wait time has passed.
	return NoMoreMovementWaitTime.CheckAfterWaitTime(GetPawn()->GetVelocity().Size() == 0);
}

void AGuardAIController::MoveToNextPatrolPointOnEQSExecuted(TSharedPtr<FEnvQueryResult> Result)
{
	NextPatrolPoint = Result->GetItemAsLocation(0);
	MoveToLocation(NextPatrolPoint, 10);
}
