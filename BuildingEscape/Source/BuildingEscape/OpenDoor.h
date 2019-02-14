// Copyright 2019 Arbel Industries

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere) float OpenAngle = -90.f;
    UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlate;
    UPROPERTY(EditAnywhere) float DoorCloseDelay = 1.f;
    UPROPERTY(EditAnywhere) float MassOnPlateToOpen = 30.f;
    float LastDoorOpenTime;
    
    AActor* Owner; // the owning door
    
    void OpenTheDoor();
    void CloseTheDoor();
    float GetTotalMassOfActorsOnPlate();
    
};
