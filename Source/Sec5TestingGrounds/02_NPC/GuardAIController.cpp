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
	
	if (!ensure(EQSQuery)) return;
	FindNextWaypointEQSRequest = FEnvQueryRequest(EQSQuery, this);
}

void AGuardAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ActualGuardBehavior == EGuardBahaviorState::NoOngoingAction) {
		MovementStartedAt = GetWorld()->GetTimeSeconds();
		FindNextWaypointEQSRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AGuardAIController::MoveToNextWaypoint);
		ActualGuardBehavior = EGuardBahaviorState::MovingToTheNextWaypoint;
		return;
	}

	if (ActualGuardBehavior == EGuardBahaviorState::MovingToTheNextWaypoint) {
		if (GetPawn()->GetVelocity().Size() == 0 && GetWorld()->GetTimeSeconds() - MovementStartedAt >= 1) { // Need to wait a second before checking Velocity, because at the beginning of the movement the Velocity is 0
			WaitStartedAt = GetWorld()->GetTimeSeconds();
			ActualWaitTime = FMath::FRandRange(MinWaitTime, MaxWaitTime);
			ActualGuardBehavior = EGuardBahaviorState::Waiting;
			return;
		}
	}

	if (ActualGuardBehavior == EGuardBahaviorState::Waiting) {
		if (GetWorld()->GetTimeSeconds() - WaitStartedAt >= ActualWaitTime) {
			WaitStartedAt = 0;
			ActualWaitTime = 0;
			ActualGuardBehavior = EGuardBahaviorState::NoOngoingAction;
			return;
		}
	}
}

void AGuardAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor: %s ||| Stimulus: %i"), *Actor->GetName(), Stimulus.WasSuccessfullySensed());
}

void AGuardAIController::MoveToNextWaypoint(TSharedPtr<FEnvQueryResult> Result)
{
	NextWaypoint = Result->GetItemAsLocation(0);
	MoveToLocation(NextWaypoint, 10);
}
