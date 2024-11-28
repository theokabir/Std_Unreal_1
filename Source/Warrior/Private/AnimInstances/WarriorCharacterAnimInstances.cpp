// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/WarriorCharacterAnimInstances.h"

#include "KismetAnimationLibrary.h"
#include "Character/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UWarriorCharacterAnimInstances::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwnerMovement = OwningCharacter->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstances::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwnerMovement) return;

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	HasAcceleration = OwnerMovement->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
