// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRoute.generated.h"

/**
 * A "route card" to help AI choose their next waypoint
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEC5TESTINGGROUNDS_API UPatrolRoute : public UActorComponent
{
	GENERATED_BODY()

public:
	UPatrolRoute();
	TArray<AActor*> GetPatrolPoints() const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
	TArray<AActor*> PatrolPoints;
};
