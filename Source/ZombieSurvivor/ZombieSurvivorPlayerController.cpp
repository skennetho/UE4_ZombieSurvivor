// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivorPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ZombieSurvivorCharacter.h"
#include "Engine/World.h"

AZombieSurvivorPlayerController::AZombieSurvivorPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}


void AZombieSurvivorPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// TODO: change it to "look at target"
	rotateToMouseLocation();
}

void AZombieSurvivorPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AZombieSurvivorPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AZombieSurvivorPlayerController::OnSetDestinationReleased);

	// test: bind axis
	InputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AZombieSurvivorPlayerController::MoveHorizontal);
	InputComponent->BindAxis(TEXT("MoveVertical"), this, &AZombieSurvivorPlayerController::MoveVertical);

}

void AZombieSurvivorPlayerController::MoveVertical(float Value)
{
	GetPawn()->AddMovementInput(FVector::XAxisVector, Value);
}

void AZombieSurvivorPlayerController::MoveHorizontal(float Value)
{
	GetPawn()->AddMovementInput(FVector::YAxisVector, Value);
}

void AZombieSurvivorPlayerController::rotateToMouseLocation()
{
	if (AZombieSurvivorCharacter* MyPawn = Cast<AZombieSurvivorCharacter>(GetPawn()))
	{
		FVector vector = MyPawn->GetCursorToWorld()->GetComponentLocation() - MyPawn->GetActorLocation();
		
		MyPawn->SetActorRotation(FRotator(0, vector.Rotation().Yaw, 0));
	}

}


void AZombieSurvivorPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AZombieSurvivorPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AZombieSurvivorPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
