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
	FWaitTime(UObject* WorldContextObjectToSet, float TimeToWaitToSet);
	FWaitTime(UObject* WorldContextObjectToSet, float MinTimeToWaitToSet, float MaxTimeToWaitToSet);

	bool HasPassed();
	bool CheckAfterWaitTime(bool ConditionToCheck);
	void Clear();

private:
	UObject* WorldContextObject;
	bool InUse;
	bool WaitTimeIsRandom;
	float TimeToWait;
	float WaitStartedAt;
	float MinTimeToWait;
	float MaxTimeToWait;
};
