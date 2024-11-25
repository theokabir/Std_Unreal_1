#include "Components/Combat/HeroCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DebugHelper.h"
#include "WarriorGameplayTags.h"
#include "Items/Weapons/WarriorHeroWeapon.h"

AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponTag(FGameplayTag InWeaponTag) const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) return;
	OverlappedActors.AddUnique(HitActor);
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Shared_Event_MelleHit,
		Data
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
