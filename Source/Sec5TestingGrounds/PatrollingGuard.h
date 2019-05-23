// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "PatrollingGuard.generated.h"

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API APatrollingGuard : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()
	
public: // TODO Create getter
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
	TArray<AActor*> PatrolPoints;
};
