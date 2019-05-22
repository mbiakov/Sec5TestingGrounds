// MBI Copyrights

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) {
	UBlackboardComponent * BlackboardComponent = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);

	return EBTNodeResult::Succeeded;
}
