// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ground.generated.h"

UCLASS()
class SEC5TESTINGGROUNDS_API AGround : public AActor
{
	GENERATED_BODY()
	
public:	
	AGround();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Ground Generation")
	void PlaceActors(TSubclassOf<AActor> ActorToSpawn, int32 MinSpawn, int32 MaxSpawn);

private:
	bool CastSphere(FVector Location, float Radius);
};
