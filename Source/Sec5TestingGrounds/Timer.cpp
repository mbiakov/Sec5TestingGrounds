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

	if (!TimerPtrPtr) {
		UTimer* TimerToAdd = NewObject<UTimer>();
		TimerToAdd->InitializeTimer(WorldContextObject, TimeToWait);
		TimerMap.Add(UniqueTimerName, TimerToAdd);
		return false;
	}

	// The Timer was already set, so we need to verify if the time has passed
	if ((*TimerPtrPtr)->TimeHasPassed(WorldContextObject)) {
		TimerMap.Remove(UniqueTimerName);
		return true;
	}
	
	// Timer->TimeHasPassed returned false so the Time has not passed
	return false;
}

bool FTimerUtility::TimeHasPassed(UObject* WorldContextObject, float MinTimeToWait, float MaxTimeToWait, FName UniqueTimerName)
{
	float TimeToWait = FMath::FRandRange(MinTimeToWait, MaxTimeToWait);
	return TimeHasPassed(WorldContextObject, TimeToWait, UniqueTimerName);
}

bool FTimerUtility::CheckAfterWaitTime(UObject* WorldContextObject, bool ConditionToCheck, float TimeToWait, FName UniqueTimerName)
{
	UTimer** TimerPtrPtr = TimerMap.Find(UniqueTimerName);

	if (!TimerPtrPtr) {
		UTimer* TimerToAdd = NewObject<UTimer>();
		TimerToAdd->InitializeTimer(WorldContextObject, TimeToWait);
		TimerMap.Add(UniqueTimerName, TimerToAdd);
		return false;
	}

	// The Timer was already set, so we need to verify if the time has passed
	if ((*TimerPtrPtr)->TimeHasPassed(WorldContextObject)) {
		if (!ConditionToCheck) return false; // The time has passed but the condition is false. We keep the Timer to be able to know that the time has passed
		// The time has passed and condition is true, the timer can be deleted
		TimerMap.Remove(UniqueTimerName);
		return true;
	}

	// Timer->TimeHasPassed returned false so the Time has not passed
	return false;
}

void FTimerUtility::ClearTimers()
{
	TimerMap.Empty();
}
