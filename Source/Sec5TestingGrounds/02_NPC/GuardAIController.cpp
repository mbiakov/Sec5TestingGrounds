// MBI Copyrights

#include "GuardAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

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

void AGuardAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ActualGuardBehavior == EGuardBahaviorState::NoOngoingAction) {
		FindNextPatrolPointEQSRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AGuardAIController::MoveToNextPatrolPoint);
		ActualGuardBehavior = EGuardBahaviorState::MovingToTheNextPatrolPoint;
		return;
	}

	if (ActualGuardBehavior == EGuardBahaviorState::MovingToTheNextPatrolPoint) {
		if (NoMoreMovement()) {
			ActualGuardBehavior = EGuardBahaviorState::WaitingOnPatrolPoint;
			return;
		}
	}

	if (ActualGuardBehavior == EGuardBahaviorState::WaitingOnPatrolPoint) {
		if (Timer.TimeHasPassed(this, MinPatrolPointWaitTime, MaxPatrolPointWaitTime, "WaitOnPatrolPoint")) {
			ActualGuardBehavior = EGuardBahaviorState::NoOngoingAction;
			return;
		}
	}
}

void AGuardAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor: %s ||| Stimulus: %i"), *Actor->GetName(), Stimulus.WasSuccessfullySensed());
}

bool AGuardAIController::NoMoreMovement()
{
	// We need to wait before checking for Velocity, because at the beginning of the movement the Velocity is 0.
	// If we don't wait before checking, the Guard State will go to Waiting directly at the Begging of the Movement
	// Wait Time count will start, while moving. When the Guard will arrive to destination, he will not wait anymore because the wait time has passed.
	return Timer.CheckAfterWaitTime(this, GetPawn()->GetVelocity().Size() == 0, 1, "WaitForMovementStart");
}

void AGuardAIController::MoveToNextPatrolPoint(TSharedPtr<FEnvQueryResult> Result)
{
	NextPatrolPoint = Result->GetItemAsLocation(0);
	MoveToLocation(NextPatrolPoint, 10);
}
