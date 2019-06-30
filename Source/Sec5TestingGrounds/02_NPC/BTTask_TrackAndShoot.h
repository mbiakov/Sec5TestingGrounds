// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WaitTime.h"
#include "BTTask_TrackAndShoot.generated.h"

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API UBTTask_TrackAndShoot : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTask_TrackAndShoot();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	/** Shooting Configuration */
	// If true, the AI will shoot a Burst as soon as the Player is detected. If false, the AI will first wait for the time specified by BetweenBurstsWaitTime.
	UPROPERTY(EditAnywhere, Category = "Shooting Configuration")
	bool MustShootABurst = true;
	UPROPERTY(EditAnywhere, Category = "Shooting Configuration")
	float MinTimeBetweenTwoBursts = 1;
	UPROPERTY(EditAnywhere, Category = "Shooting Configuration")
	float MaxTimeBetweenTwoBursts = 4;
	UPROPERTY(EditAnywhere, Category = "Shooting Configuration")
	int MinShootsInBurst = 2;
	UPROPERTY(EditAnywhere, Category = "Shooting Configuration")
	int MaxShootsInBurst = 5;
	UPROPERTY(EditAnywhere, Category = "Shooting Configuration")
	float ShootFrequencyInBurst = 0.15;

	/** Shooting Utilities */
	void ShootAtEnemy();
	FWaitTime BetweenBurstsWaitTime = FWaitTime(this, MinTimeBetweenTwoBursts, MaxTimeBetweenTwoBursts);
	FWaitTime BetweenShootsWaitTime = FWaitTime(this, ShootFrequencyInBurst);
	int32 ShootsInBurst = 0;
	int32 ActualShootInBurst = 0;

	/** Utilities */
	class AAIController* AIController;
	class AUnitedCharacter* AICharacter;
	UBlackboardComponent* Blackboard;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector EnemyKey;
};
