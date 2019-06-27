// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "WaitTime.generated.h"

/**
 * Comment
 */
USTRUCT()
struct FWaitTime
{
	GENERATED_USTRUCT_BODY()

public:
	FWaitTime();
	FWaitTime(UObject* WorldContextObjectToSet);

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
