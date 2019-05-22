// MBI Copyrights

#include "ChooseNextWaypoint.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) {
	UE_LOG(LogTemp, Warning, TEXT("CPP Execute Task"));
	return EBTNodeResult::Succeeded;
}
