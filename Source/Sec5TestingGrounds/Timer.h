// MBI Copyrights

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Timer.generated.h"

/**
 * 
 */
UCLASS()
class SEC5TESTINGGROUNDS_API UTimer : public UObject
{
	GENERATED_BODY()
	
public:
	UTimer();
	void InitializeTimer(UObject* WorldContextObject, float TimeToWaitToSet);
	bool TimeHasPassed(UObject* WorldContextObject);

private:
	float TimeToWait;
	float WaitStartedAt;
};

/**
 * Timer Utility
 */
USTRUCT()
struct FTimerUtility
{
	GENERATED_USTRUCT_BODY()

public:
	bool TimeHasPassed(UObject* WorldContextObject, float TimeToWait, FName UniqueTimerName);
	bool TimeHasPassed(UObject* WorldContextObject, float MinTimeToWait, float MaxTimeToWait, FName UniqueTimerName);
	bool CheckAfterWaitTime(UObject* WorldContextObject, bool ConditionToCheck, float TimeToWait, FName UniqueTimerName);
	void ClearTimers();

private:
	TMap<FName, UTimer*> TimerMap;
	bool SetNewTimer(UObject* WorldContextObject, float TimeToWait, FName UniqueTimerName);
	bool CheckAndRemoveTimerIfTimeHasPassed(UObject* WorldContextObject, UTimer* Timer, FName UniqueTimerName);
};
