// Copyright Toby Elgin 2017.

#include "OpenDoor.h"
#include "BuildingEscape.h"
#include "Components/PrimitiveComponent.h"
#include <GameFramework/Actor.h>

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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOnPlate() 
{
	float TotalMass = 0.f;

	if (!PressurePlate) { return TotalMass; }

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	FString ActorNames = "";

	// Iterate through them and adding their masses
	for (const auto* Actor: OverlappingActors)
	{
		ActorNames = ActorNames + Actor->GetName() + " ";
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	UE_LOG(LogTemp, Warning, TEXT("%s on plate"), *ActorNames)

	return TotalMass;
}