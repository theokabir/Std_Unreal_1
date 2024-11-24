#include "DataAssets/StartupData/DataAsset_Enemy_StartupDatabase.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

void UDataAsset_Enemy_StartupDatabase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InWarriorASCToGive,
                                                                    int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InWarriorASCToGive, ApplyLevel);

	if (EnemyCombatAbilities.IsEmpty()) return;

	for (TSubclassOf<UWarriorEnemyGameplayAbility>& Ability: EnemyCombatAbilities)
	{
		if (!Ability) continue;
		FGameplayAbilitySpec Spec(Ability);
		Spec.SourceObject = InWarriorASCToGive->GetAvatarActor();
		Spec.Level = ApplyLevel;

		InWarriorASCToGive->GiveAbility(Spec);
	}
	
}
