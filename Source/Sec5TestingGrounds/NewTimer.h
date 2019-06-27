// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "NewTimer.generated.h"

/**
 * Comment
 */
USTRUCT()
struct FNewTimer
{
	GENERATED_USTRUCT_BODY()

public:
	FNewTimer();
	FNewTimer(UObject* WorldContextObjectToSet);

	bool TimeHasPassed(float TimeToWaitToSet);
	bool TimeHasPassed(float MinTimeToWait, float MaxTimeToWait);
	bool CheckAfterWaitTime(bool ConditionToCheck, float TimeToWaitToSet);
	void Clear();

private:
	UObject* WorldContextObject;
	bool InUse;
	float TimeToWait;
	float WaitStartedAt;
};
