// Copyright 2019 Arbel Industries

#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
    Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
        OpenTheDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    float TimeElapsed = GetWorld()->GetTimeSeconds() - LastDoorOpenTime;
    if (TimeElapsed > DoorCloseDelay) {
        CloseTheDoor();
    }
}

void UOpenDoor::OpenTheDoor()
{
    // set the door rotation
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
    
    //UE_LOG(LogTemp,Warning,TEXT("Opening The Door!!!"));
    
}

void UOpenDoor::CloseTheDoor()
{
    // set the door rotation
    Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
    
    //UE_LOG(LogTemp,Warning,TEXT("Opening The Door!!!"));
    
}
