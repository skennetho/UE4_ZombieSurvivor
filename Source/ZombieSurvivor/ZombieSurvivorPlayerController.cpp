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

	rotateToMouseLocation();
}

void AZombieSurvivorPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AZombieSurvivorPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AZombieSurvivorPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AZombieSurvivorPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AZombieSurvivorPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AZombieSurvivorPlayerController::OnResetVR);

	// test: bind axis
	InputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AZombieSurvivorPlayerController::MoveHorizontal);
	InputComponent->BindAxis(TEXT("MoveVertical"), this, &AZombieSurvivorPlayerController::MoveVertical);
}

void AZombieSurvivorPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
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

void AZombieSurvivorPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AZombieSurvivorCharacter* MyPawn = Cast<AZombieSurvivorCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AZombieSurvivorPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
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
