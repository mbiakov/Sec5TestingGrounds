// MBI Copyrights

#pragma once

#include "Perception/AIPerceptionTypes.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAIController.generated.h"

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGuardAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;

	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

private:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
