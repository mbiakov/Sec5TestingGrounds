// MBI Copyrights


#include "WaitTime.h"
#include "Engine/World.h"

FWaitTime::FWaitTime()
{
	// Never use this constructor
	ensure(false);
}

FWaitTime::FWaitTime(UObject* WorldContextObjectToSet, float TimeToWaitToSet)
{
	WorldContextObject = WorldContextObjectToSet;
	InUse = false;
	WaitTimeIsRandom = false;
	TimeToWait = TimeToWaitToSet;
	WaitStartedAt = 0;
	MinTimeToWait = 0;
	MaxTimeToWait = 0;
}

FWaitTime::FWaitTime(UObject* WorldContextObjectToSet, float MinTimeToWaitToSet, float MaxTimeToWaitToSet)
{
	WorldContextObject = WorldContextObjectToSet;
	InUse = false;
	WaitTimeIsRandom = true;
	TimeToWait = 0;
	WaitStartedAt = 0;
	MinTimeToWait = MinTimeToWaitToSet;
	MaxTimeToWait = MaxTimeToWaitToSet;
}

bool FWaitTime::HasPassed()
{
	if (!InUse) { // Initialize Timer
		if (WaitTimeIsRandom) TimeToWait = FMath::FRandRange(MinTimeToWait, MaxTimeToWait);
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

bool FWaitTime::CheckAfterWaitTime(bool ConditionToCheck)
{
	if (!InUse) { // Initialize Timer
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
