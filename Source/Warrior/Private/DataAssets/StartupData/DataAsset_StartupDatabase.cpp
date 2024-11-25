#include "DataAssets/StartupData/DataAsset_StartupDatabase.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"

void UDataAsset_StartupDatabase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InWarriorASCToGive,
                                                              int32 ApplyLevel)
{
	check(InWarriorASCToGive);
	
	GrantAbilities(ActivateOnGivenAbilities, InWarriorASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InWarriorASCToGive, ApplyLevel);

	if (!StartupGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& Effect: StartupGameplayEffects)
		{
			if (!Effect) continue;
			const UGameplayEffect* EffectCDO = Effect->GetDefaultObject<UGameplayEffect>();
			InWarriorASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InWarriorASCToGive->MakeEffectContext()
			);
		}	
	}
}

void UDataAsset_StartupDatabase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive,
	UWarriorAbilitySystemComponent* InWarriorASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty()) return;

	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec Spec(Ability);
		Spec.SourceObject = InWarriorASCToGive->GetAvatarActor();
		Spec.Level = ApplyLevel;
		
		InWarriorASCToGive->GiveAbility(Spec);
	}
}
