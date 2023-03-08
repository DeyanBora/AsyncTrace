
UFUNCTION(BlueprintCallable)
void SetWantsUseOverlap();

void ReceiveOnUseOverlapCompleted(const TArray<FOverlapResult>& Results);

FTraceHandle RequestUseOverlap(); // Actually do Overlap request

void OnUseOverlapCompleted(const FTraceHandle& Handle, FOverlapDatum& Data);	//Delegate function
void DoWorkWithUseOverlapResults(const FOverlapDatum& OverlapData);

FTraceHandle LastUseOverlapHandle;
FOverlapDelegate UseOverlapDelegate; //Delegate fired when trace is completed
uint32 bWantsUseOverlap : 1; 