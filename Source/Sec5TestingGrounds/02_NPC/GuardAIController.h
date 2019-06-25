// MBI Copyrights

#pragma once

#include "Perception/AIPerceptionTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
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
	virtual void BeginPlay() override;

	// Perceprion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

	// EQS
	UPROPERTY(EditDefaultsOnly, Category = "EQS")
	class UEnvQuery* EQSQuery;

private:
	// Perceprion
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// EQS
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);
	FEnvQueryRequest EQSRequest;
};
