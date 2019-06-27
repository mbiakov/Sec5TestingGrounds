// MBI Copyrights


#include "WaitTime.h"
#include "Engine/World.h"

FWaitTime::FWaitTime()
{
	// Never use this constructor
	ensure(false);
}

FWaitTime::FWaitTime(UObject* WorldContextObjectToSet)
{
	WorldContextObject = WorldContextObjectToSet;
	InUse = false;
	TimeToWait = 0;
	WaitStartedAt = 0;
}

bool FWaitTime::TimeHasPassed(float TimeToWaitToSet)
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

bool FWaitTime::TimeHasPassed(float MinTimeToWait, float MaxTimeToWait)
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

bool FWaitTime::CheckAfterWaitTime(bool ConditionToCheck, float TimeToWaitToSet)
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

void FWaitTime::Clear()
{
	InUse = false;
}
