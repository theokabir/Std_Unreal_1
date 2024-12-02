#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "WarriorGameplayTags.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorTypes/WarriorStructTypes.h"


void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;
		if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable))
			if (AbilitySpec.IsActive())
				CancelAbilityHandle(AbilitySpec.Handle);
			else
				TryActivateAbility(AbilitySpec.Handle);
		else
			TryActivateAbility(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld)) return;

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && Spec.IsActive())
		{
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbility(const TArray<FWarriorHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FWarriorHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(AbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = ApplyLevel;
		Spec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		OutHandles.AddUnique(GiveAbility(Spec));
	}
}

void UWarriorAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty()) return;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (!SpecHandle.IsValid()) return;
		ClearAbility(SpecHandle);
	}

	InSpecHandlesToRemove.Empty();
}

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag InAbilityTag)
{
	check(InAbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundSpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(InAbilityTag.GetSingleTagContainer(), FoundSpecs);

	if (FoundSpecs.IsEmpty()) return false;
	
	const int32 RandomAbilityIndex = FMath::FRandRange(0.f, FoundSpecs.Num() - 1);
	const FGameplayAbilitySpec* Spec = FoundSpecs[RandomAbilityIndex];
	check(Spec);
	if (Spec->IsActive()) return false;
	return TryActivateAbility(Spec->Handle);
}
