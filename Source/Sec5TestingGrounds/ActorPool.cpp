// MBI Copyrights

#include "ActorPool.h"


UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = false;
}


AActor* UActorPool::Checkout()
{
	if (ActorPool.Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("%s UActorPool::Checkout() doesn't have any available Actor in Pool"), *this->GetName());
		return nullptr;
	}
	return ActorPool.Pop();
}

void UActorPool::ReturnToPool(AActor* ActorToReturn)
{
	AddToPool(ActorToReturn);
}

void UActorPool::AddToPool(AActor* ActorToAdd)
{
	ActorPool.Add(ActorToAdd);
}
