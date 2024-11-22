// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "DebugHelper.h"
#include "Character/WarriorHeroCharacter.h"

void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (OwningCharacter)
	{
		OwningHeroCharacter = CastChecked<AWarriorHeroCharacter>(OwningCharacter);
	}
}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (HasAcceleration)
	{
		RelaxAnimationTimeElapse = 0;
		bShouldRelaxAnimation = false;
	}
	else
	{
		RelaxAnimationTimeElapse += DeltaSeconds;
		bShouldRelaxAnimation = RelaxAnimationTimeElapse >= RelaxAnimationThreshold;
	}
}
