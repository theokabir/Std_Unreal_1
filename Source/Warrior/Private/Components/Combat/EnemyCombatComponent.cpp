#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) return;
	OverlappedActors.AddUnique(HitActor);

	// TODO: implement block check
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// TODO: Check if the block is valid
	}
	
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	Data.EventTag = WarriorGameplayTags::Shared_Event_MelleHit;
	
	if (bIsValidBlock)
	{
		// TODO: Handle Successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			WarriorGameplayTags::Shared_Event_MelleHit,
			Data
		);
	}
}
