// MBI Copyrights


#include "BTTask_TrackAndShoot.h"
#include "Classes/AIController.h"
#include "Character/UnitedCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TrackAndShoot::UBTTask_TrackAndShoot()
{
	bNotifyTick = true;
	ForceInstancing(true);
}

EBTNodeResult::Type UBTTask_TrackAndShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Initialize
	AIController = OwnerComp.GetAIOwner();
	AICharacter = Cast<AUnitedCharacter>(AIController->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();

	return EBTNodeResult::InProgress;
}

void UBTTask_TrackAndShoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// Track Enemy
	AIController->MoveToActor(Cast<AActor>(Blackboard->GetValueAsObject(EnemyKey.SelectedKeyName)), 300);
	ShootAtEnemy();
}

void UBTTask_TrackAndShoot::ShootAtEnemy()
{
	if (!MustShootABurst) {
		if (BetweenBurstsWaitTime.HasPassed()) MustShootABurst = true;
	}

	if (MustShootABurst) {
		// Initialization for each burst
		if (ShootsInBurst == 0) ShootsInBurst = FMath::RandRange(MinShootsInBurst, MaxShootsInBurst);

		if (BetweenShootsWaitTime.HasPassed()) {
			AICharacter->PullTrigger();

			ActualShootInBurst++;
			if (ActualShootInBurst >= ShootsInBurst) { // Burst finished, reinitialize shooting variables
				MustShootABurst = false;
				ShootsInBurst = 0;
				ActualShootInBurst = 0;
			}
		}
	}
}
