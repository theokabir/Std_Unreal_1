// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartupData/DataAsset_HeroStartupData.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorTypes/WarriorStructTypes.h"

void UDataAsset_HeroStartupData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InWarriorASCToGive,
	int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InWarriorASCToGive, ApplyLevel);
	for (const FWarriorHeroAbilitySet& AbilitySet: HeroStartAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec Spec(AbilitySet.AbilityToGrant);
		Spec.SourceObject = InWarriorASCToGive->GetAvatarActor();
		Spec.Level = ApplyLevel;
		Spec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		InWarriorASCToGive->GiveAbility(Spec);
	}
}
