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

	// Initialize usefull variables
	ControlledCharacter = Cast<AUnitedCharacter>(InPawn);
}

void AGuardAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// State Transitions
	if (DetectedEnemy) {
		MoveToActor(DetectedEnemy, 300);
		Timer.ClearTimers(); // Since we enter EnemyDetected state globally, there may remain some Timers. We need to clear them.
		ActualGuardBehavior = EGuardBahaviorState::EnemyDetected;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::EnemyDetected && !DetectedEnemy) {
		StopMovement();
		ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::MovingToTheNextPatrolPoint && NoMoreMovement()) {
		ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
	}
	if (ActualGuardBehavior == EGuardBahaviorState::WaitingOnPatrolPoint && Timer.TimeHasPassed(this, MinPatrolPointWaitTime, MaxPatrolPointWaitTime, "WaitOnPatrolPoint")) {
		FindNextPatrolPointEQSRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AGuardAIController::MoveToNextPatrolPointOnEQSExecuted);
		ActualGuardBehavior = EGuardBahaviorState::MovingToTheNextPatrolPoint;
	}

	// State Actions
	if (ActualGuardBehavior == EGuardBahaviorState::EnemyDetected) {
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
		if (ensure(ControlledCharacter)) ControlledCharacter->MustAim = true;
	}
	if (!Stimulus.WasSuccessfullySensed()) {
		DetectedEnemy = nullptr;
		ClearFocus(EAIFocusPriority::Gameplay);
		if (ensure(ControlledCharacter)) ControlledCharacter->MustAim = false;
	}
}

void AGuardAIController::ShootAtEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("%f: Shooting Enemy"), GetWorld()->GetTimeSeconds());
}

bool AGuardAIController::NoMoreMovement()
{
	// We need to wait before checking for Velocity, because at the beginning of the movement the Velocity is 0.
	// If we don't wait before checking, the Guard State will go to Waiting directly at the Begging of the Movement
	// Wait Time count will start, while moving. When the Guard will arrive to destination, he will not wait anymore because the wait time has passed.
	return Timer.CheckAfterWaitTime(this, GetPawn()->GetVelocity().Size() == 0, 1, "WaitForMovementStart");
}

void AGuardAIController::MoveToNextPatrolPointOnEQSExecuted(TSharedPtr<FEnvQueryResult> Result)
{
	NextPatrolPoint = Result->GetItemAsLocation(0);
	MoveToLocation(NextPatrolPoint, 10);
}
