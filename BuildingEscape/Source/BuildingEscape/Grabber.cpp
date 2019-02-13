// Copyright 2019 Arbel Industries

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();

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
    
    // if the physics handle is attached
    if (PhysicsHandle->GrabbedComponent) {
        // move the object we're holding
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
    
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp,Warning,TEXT("Grabbed by %s"), *GetOwner()->GetName());
    /// LINE TRACE and reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    /// if we hit something then attach a physics handle
    if (ActorHit) {
        PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
    }
}

void UGrabber::Release()
{
    UE_LOG(LogTemp,Warning,TEXT("Released by %s"), *GetOwner()->GetName());
    PhysicsHandle->ReleaseComponent();
}

/// look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle) {
        /// found physics handle
        /// UE_LOG(LogTemp,Warning,TEXT("Yay - found the physics handle component for %s"), *GetOwner()->GetName());
    } else {
        UE_LOG(LogTemp,Error,TEXT("%s missing physics handle component"), *GetOwner()->GetName());
    }
}

/// setup the input component
void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent) {
        /// bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    } else {
        UE_LOG(LogTemp,Error,TEXT("%s missing input component"), *GetOwner()->GetName());
    }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    /// get player view point
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
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
        //UE_LOG(LogTemp,Warning,TEXT("I am hitting this thing: %s"), *(ActorHit->GetName()));
    }
    return Hit;
}
