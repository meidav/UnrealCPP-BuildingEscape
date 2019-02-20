// Copyright 2019 Arbel Industries

#include "OpenDoor.h"

#define OUT

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
    Owner = GetOwner();
    if (!PressurePlate) {
        UE_LOG(LogTemp,Error,TEXT("%s has no PressurePlate"), *Owner->GetName());
    }
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetTotalMassOfActorsOnPlate() > MassOnPlateToOpen) { 
        OnOpen.Broadcast();
    } else {
        OnClose.Broadcast();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.;
    
    /// find all the overlapping actors
    TArray<AActor*> OverlappingActors;
    if (!PressurePlate) {
        return TotalMass;
    }
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    /// iterate through them adding their masses to the total
    for (const auto& Actor : OverlappingActors) {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        //UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
    }
    
    return TotalMass;
}
