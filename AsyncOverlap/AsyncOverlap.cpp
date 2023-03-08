#include "Engine/World.h"
void AAsyncOverlap::Tick(float DeltaTime)
{
	if (LastHitOverlapHandle._Data.FrameNumber != 0)
		{
			FOverlapDatum OutData;
			if (GetWorld()->QueryOverlapData(LastHitOverlapHandle, OutData))
			{
				// Clear out handle so next tick we don't enter
				LastHitOverlapHandle._Data.FrameNumber = 0;
				// trace is finished, do stuff with results
				DoWorkWithHitOverlapResults(OutData);
			}
		}

	if (bWantsHitOverlap)
	{
		LastHitOverlapHandle = RequestHitOverlap();
		bWantsHitOverlap = false;
	}
}
FTraceHandle AAsyncOverlap::RequestUseOverlap()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return FTraceHandle();
	}

	FCollisionObjectQueryParams ObjectQueryParameters(ECC_Pawn);

	bool bTraceComplex = false;
	bool bIgnoreSelf = true;

	FVector Start = GetMesh()->GetSocketLocation(FName("OverlapSocketLocation"));
	FQuat Rot = FQuat::Identity;
	float AttackRaycastRadius = 15.f;

	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(AttackRaycastRadius);

	return World->AsyncOverlapByObjectType(
		Start,
		Rot,
		ObjectQueryParameters,
		CollisionShape,
		FCollisionQueryParams::DefaultQueryParam,
		&UseOverlapDelegate
	);

	//For Sync Function use this instance
	//TArray<AActor*> ActorsToIgnore;
	//TArray<AActor*> OutActors;
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Start, 15, SyncOverlapObjectType, APawn::GetClass(), ActorsToIgnore, OutActors);
}

//Function for Requesting overlaps 
void AAsyncOverlap::SetWantsUseOverlap()
{
	//true means Overlap false means Trace
	if (!GetWorld()->IsTraceHandleValid(LastUseOverlapHandle, true))
	{
		bWantsUseOverlap = true;
	}
}

//Just resetter function for async system
void AAsyncOverlap::OnUseOverlapCompleted(const FTraceHandle& Handle, FOverlapDatum& Data)
{
	ensure(Handle == LastUseOverlapHandle);
	DoWorkWithUseOverlapResults(Data);
	LastUseOverlapHandle._Data.FrameNumber = 0; //To reset it
}

//Data Transfer function for next frame
void AAsyncOverlap::DoWorkWithUseOverlapResults(const FOverlapDatum& OverlapData)
{
	//Do async operations inside of ReceiveOnUseOverlapCompleted instead of here
	ReceiveOnUseOverlapCompleted(OverlapData.OutOverlaps);
}
