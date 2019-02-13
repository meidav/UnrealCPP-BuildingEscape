// Copyright 2019 Arbel Industries

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Warning,TEXT("Grabber reporting for duty!!!"));
    
    /// look for attached physics handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle) {
        /// found physics handle
        UE_LOG(LogTemp,Warning,TEXT("Yay - found the physics handle component for %s"), *GetOwner()->GetName());
    } else {
        UE_LOG(LogTemp,Error,TEXT("%s missing physics handle component"), *GetOwner()->GetName());
    }
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// get player view point
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(10,0,230),
        false,
        0.f,
        0.f,
        10.f
    );
    
    /// Setup query params
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    /// LineTrace (aka ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            PlayerViewPointLocation,
                                            LineTraceEnd,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    
    /// see what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit) {
        UE_LOG(LogTemp,Warning,TEXT("I am hitting this thing: %s"), *(ActorHit->GetName()));
    }
}

