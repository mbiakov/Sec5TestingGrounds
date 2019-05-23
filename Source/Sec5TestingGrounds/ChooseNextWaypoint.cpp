// MBI Copyrights

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrollingGuard.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) {
	// TODO Protect against empty Patrol Route

	/// Get the Patrol Points from the controlled PatrollingGuard
	TArray<AActor*> PatrolPoints = Cast<APatrollingGuard>(OwnerComp.GetAIOwner()->GetPawn())->PatrolPoints;

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
