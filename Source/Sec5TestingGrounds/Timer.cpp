// MBI Copyrights


#include "Timer.h"
#include "Engine/World.h"

UTimer::UTimer()
{
}

void UTimer::InitializeTimer(UObject* WorldContextObject, float TimeToWaitToSet)
{
	TimeToWait = TimeToWaitToSet;
	WaitStartedAt = WorldContextObject->GetWorld()->GetTimeSeconds();
}

bool UTimer::TimeHasPassed(UObject* WorldContextObject)
{
	return (WorldContextObject->GetWorld()->GetTimeSeconds() - WaitStartedAt >= TimeToWait);
}

bool FTimerUtility::TimeHasPassed(UObject* WorldContextObject, float TimeToWait, FName UniqueTimerName)
{
	UTimer** TimerPtrPtr = TimerMap.Find(UniqueTimerName);
	if (!TimerPtrPtr) return SetNewTimer(WorldContextObject, TimeToWait, UniqueTimerName);
	return CheckAndRemoveTimerIfTimeHasPassed(WorldContextObject, (*TimerPtrPtr), UniqueTimerName);
}

bool FTimerUtility::TimeHasPassed(UObject* WorldContextObject, float MinTimeToWait, float MaxTimeToWait, FName UniqueTimerName)
{
	UTimer** TimerPtrPtr = TimerMap.Find(UniqueTimerName);
	if (!TimerPtrPtr) return SetNewTimer(WorldContextObject, FMath::FRandRange(MinTimeToWait, MaxTimeToWait), UniqueTimerName);
	return CheckAndRemoveTimerIfTimeHasPassed(WorldContextObject, (*TimerPtrPtr), UniqueTimerName);
}

bool FTimerUtility::CheckAfterWaitTime(UObject* WorldContextObject, bool ConditionToCheck, float TimeToWait, FName UniqueTimerName)
{
	UTimer** TimerPtrPtr = TimerMap.Find(UniqueTimerName);
	if (!TimerPtrPtr) return SetNewTimer(WorldContextObject, TimeToWait, UniqueTimerName);
	// Verify the confition
	if (!ConditionToCheck) return false;
	return CheckAndRemoveTimerIfTimeHasPassed(WorldContextObject, (*TimerPtrPtr), UniqueTimerName);
}

void FTimerUtility::ClearTimers()
{
	TimerMap.Empty();
}

bool FTimerUtility::SetNewTimer(UObject* WorldContextObject, float TimeToWait, FName UniqueTimerName)
{
	UTimer* TimerToAdd = NewObject<UTimer>();
	TimerToAdd->InitializeTimer(WorldContextObject, TimeToWait);
	TimerMap.Add(UniqueTimerName, TimerToAdd);
	return false;
}

bool FTimerUtility::CheckAndRemoveTimerIfTimeHasPassed(UObject* WorldContextObject, UTimer* Timer, FName UniqueTimerName)
{
	if (Timer->TimeHasPassed(WorldContextObject)) {
		TimerMap.Remove(UniqueTimerName);
		return true;
	}

	// Timer->TimeHasPassed returned false so the Time has not passed
	return false;
}
