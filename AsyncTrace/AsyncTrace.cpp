#include "Engine/World.h"

void AAsyncTraceActor::Tick(float DeltaTime)
{
	if (LastTraceHandle._Data.FrameNumber != 0)
	{
		FTraceDatum OutData;
		if (GetWorld()->QueryTraceData(LastTraceHandle, OutData))
		{
			// Clear out handle so next tick we don't continue
			LastTraceHandle._Data.FrameNumber = 0;
			// trace is finished, do stuff with results
			DoWorkWithTraceResults(OutData);
		}
	}
	if (bWantsTrace)
	{
		LastTraceHandle = RequestTrace();
		bWantsTrace = false;
	}
}

//Work with Results
void AAsyncTraceActor::ReceiveOnTraceCompleted(const TArray<FHitResult>& Results)
{
	FHitResult OutHitResult;
	TArray<AActor*> EmptyArray;

	AActor* EnemyActorPointer = nullptr;
	if (!Results.IsEmpty())
	{
		EnemyActorPointer = Results[0].GetActor();
		//Do Work With enemy Actor Pointer

	}
}

//Tracing Operation
FTraceHandle AAsyncTraceActor::RequestTrace()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return FTraceHandle();
	}

	auto Channel = UEngineTypes::ConvertToCollisionChannel(AsyncTraceType);
	FCollisionObjectQueryParams ObjectQueryParameters(Channel);

	bool bTraceComplex = false;

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("NAME_AsyncRequestTrace"), bTraceComplex/*, this*/);
	TraceParams.AddIgnoredActor(this);

	FVector Start = GetMesh()->GetSocketLocation(FName("StartSocketName"));
	FVector End = GetMesh()->GetSocketLocation(FName("EndSocketName"));

	return World->AsyncLineTraceByChannel(EAsyncTraceType::Single,
		Start, End,
		Channel,
		TraceParams,
		FCollisionResponseParams::DefaultResponseParam,
		&TraceDelegate);
}

}

//Function for Requesting Traces (Dont use bWantsTrace=true directly!)
void AAsyncTraceActor::SetWantsTrace()
{
	if (!GetWorld()->IsTraceHandleValid(LastTraceHandle, false))
	{
		bWantsTrace = true;
	}
}

//Just Resetter for async task
void AAsyncTraceActor::OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	ensure(Handle == LastTraceHandle);
	DoWorkWithTraceResults(Data);
	LastTraceHandle._Data.FrameNumber = 0; //To reset it
}

//Data Transfer function for next frame 
void AAsyncTraceActor::DoWorkWithTraceResults(const FTraceDatum& TraceData)
{
	// do things in ReceiveOnTraceCompleted
	ReceiveOnTraceCompleted(TraceData.OutHits);
}
