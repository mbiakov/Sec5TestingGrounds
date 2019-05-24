// MBI Copyrights

#include "ChooseNextWaypoint.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) {
	/// Get the Patrol Points from the component UPatrolRoute that must be attached to the Pawn
	UPatrolRoute* PatrolRoute = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) return EBTNodeResult::Failed;
	TArray<AActor*> PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing Patrol Points"));
		return EBTNodeResult::Failed;
	}

	/// Set Next Waypoint
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	AActor* NextWaypoint = PatrolPoints[Index];
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, NextWaypoint);

	/// Cycle Index
	int32 NextWaypointIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextWaypointIndex);

	return EBTNodeResult::Succeeded;
}
