// MBI Copyrights


#include "NewTimer.h"
#include "Engine/World.h"

FNewTimer::FNewTimer()
{
	// Never use this constructor
	ensure(false);
}

FNewTimer::FNewTimer(UObject* WorldContextObjectToSet)
{
	WorldContextObject = WorldContextObjectToSet;
	InUse = false;
	TimeToWait = 0;
	WaitStartedAt = 0;
}

bool FNewTimer::TimeHasPassed(float TimeToWaitToSet)
{
	if (!InUse) { // Initialize Timer
		TimeToWait = TimeToWaitToSet;
		WaitStartedAt = WorldContextObject->GetWorld()->GetTimeSeconds();
		InUse = true;
		return false;
	}
	// Timer already initialized

	if (WorldContextObject->GetWorld()->GetTimeSeconds() - WaitStartedAt >= TimeToWait) {
		InUse = false; // To be able to use the Timer next time
		return true;
	}
	return false;
}

bool FNewTimer::TimeHasPassed(float MinTimeToWait, float MaxTimeToWait)
{
	if (!InUse) { // Initialize Timer
		TimeToWait = FMath::FRandRange(MinTimeToWait, MaxTimeToWait);
		WaitStartedAt = WorldContextObject->GetWorld()->GetTimeSeconds();
		InUse = true;
		return false;
	}
	// Timer already initialized

	if (WorldContextObject->GetWorld()->GetTimeSeconds() - WaitStartedAt >= TimeToWait) {
		InUse = false; // To be able to use the Timer next time
		return true;
	}
	return false;
}

bool FNewTimer::CheckAfterWaitTime(bool ConditionToCheck, float TimeToWaitToSet)
{
	if (!InUse) { // Initialize Timer
		TimeToWait = TimeToWaitToSet;
		WaitStartedAt = WorldContextObject->GetWorld()->GetTimeSeconds();
		InUse = true;
		return false;
	}
	// Timer already initialized

	// Verify the condition
	if (!ConditionToCheck) return false;
	
	if (WorldContextObject->GetWorld()->GetTimeSeconds() - WaitStartedAt >= TimeToWait) {
		InUse = false; // To be able to use the Timer next time
		return true;
	}
	return false;
}

void FNewTimer::Clear()
{
	InUse = false;
}
