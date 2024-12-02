#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) return;
	OverlappedActors.AddUnique(HitActor);

	// TODO: implement block check
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor, WarriorGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
		bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	Data.EventTag = WarriorGameplayTags::Shared_Event_MelleHit;
	
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			WarriorGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
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
