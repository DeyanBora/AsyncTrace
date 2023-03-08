
UFUNCTION(BlueprintCallable)
void SetWantsTrace();

void ReceiveOnTraceCompleted(const TArray<FHitResult>& Results);

FTraceHandle RequestTrace();

void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);
void DoWorkWithTraceResults(const FTraceDatum& TraceData);

UPROPERTY(EditAnywhere, BlueprintReadWrite)
TEnumAsByte<ETraceTypeQuery> AsyncTraceType;

FTraceHandle LastTraceHandle;
FTraceDelegate TraceDelegate; 
uint32 bWantsTrace : 1;