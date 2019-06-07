// MBI Copyrights

#include "ActorPool.h"


UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = false;
}


AActor* UActorPool::Checkout()
{
	return nullptr;
}

void UActorPool::ReturnToPool(AActor* ActorToReturn)
{

}

void UActorPool::Add(AActor* ActorToAdd)
{

}
