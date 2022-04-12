// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZombieSurvivorPlayerController.generated.h"

UCLASS()
class AZombieSurvivorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZombieSurvivorPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Move using W,A,S,D**/
	void MoveVertical(float Value);
	void MoveHorizontal(float Value);

	/** always look mouse cursor in e**/
	void rotateToMouseLocation();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


