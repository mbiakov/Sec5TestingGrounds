// MBI Copyrights


#include "BTService_DetectEnemiesAndFocus.h"
#include "Classes/AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/UnitedCharacter.h"

UBTService_DetectEnemiesAndFocus::UBTService_DetectEnemiesAndFocus()
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = false;
}

void UBTService_DetectEnemiesAndFocus::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// Initialize
	Blackboard = OwnerComp.GetBlackboardComponent();
	AIController = OwnerComp.GetAIOwner();
	AICharacter = Cast<AUnitedCharacter>(AIController->GetPawn());
	AIController->GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &UBTService_DetectEnemiesAndFocus::OnTargetPerceptionUpdate);
}

void UBTService_DetectEnemiesAndFocus::OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor != UGameplayStatics::GetPlayerPawn(this, 0)) return;
	// From here we know we detected the player

	if (Stimulus.WasSuccessfullySensed()) { // Player detected
		Blackboard->SetValueAsObject(EnemyKey.SelectedKeyName, Actor);
		Blackboard->SetValueAsObject(PersistentEnemyKey.SelectedKeyName, Actor);
		Blackboard->SetValueAsBool(EnemyWasSeenKey.SelectedKeyName, true);
		AIController->SetFocus(Actor, EAIFocusPriority::Gameplay);
		AICharacter->MustAim = true;
	}
	else { // Player lost
		Blackboard->ClearValue(EnemyKey.SelectedKeyName);
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		AICharacter->MustAim = false;
	}
}
