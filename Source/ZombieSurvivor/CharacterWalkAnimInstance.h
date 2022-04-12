// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterWalkAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVOR_API UCharacterWalkAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Direction = 0;
};
