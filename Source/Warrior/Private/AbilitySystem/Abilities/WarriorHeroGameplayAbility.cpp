// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "Character/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"

AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedCharacter.IsValid()) CachedCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	return CachedCharacter.IsValid() ? CachedCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedController.IsValid()) CachedController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
	return CachedController.IsValid() ? CachedController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
