// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEC5TESTINGGROUNDS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActorPool();

	AActor* Checkout();
	void ReturnToPool(AActor* ActorToReturn);
	void Add(AActor* ActorToAdd);
};
