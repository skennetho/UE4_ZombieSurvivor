// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWalkAnimInstance.h"
#include "ZombieSurvivorCharacter.h"



void UCharacterWalkAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		Speed = Pawn->GetVelocity().Size();

		AZombieSurvivorCharacter* Character = Cast<AZombieSurvivorCharacter>(Pawn);
		if (Character) {
			Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		}
	}
}

