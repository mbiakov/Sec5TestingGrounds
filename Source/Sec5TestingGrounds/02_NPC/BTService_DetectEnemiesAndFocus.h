// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Perception/AIPerceptionTypes.h"
#include "BTService_DetectEnemiesAndFocus.generated.h"

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API UBTService_DetectEnemiesAndFocus : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectEnemiesAndFocus();

protected:
	virtual void OnBecomeRelevant(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector EnemyKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector PersistentEnemyKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector EnemyWasSeenKey;

private:
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	UBlackboardComponent* Blackboard;

	class AAIController* AIController;
	class AUnitedCharacter* AICharacter;
};
