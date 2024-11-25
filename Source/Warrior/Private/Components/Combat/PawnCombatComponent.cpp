#include "Components/Combat/PawnCombatComponent.h"
#include "DebugHelper.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTag, AWarriorWeaponBase* InWeapon,
                                                 bool bRegisterAsEquiped)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTag), TEXT("Tag already used"));
	check(InWeapon);

	CharacterCarriedWeaponMap.Emplace(InWeaponTag, InWeapon);
	InWeapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
	if (bRegisterAsEquiped) CurrentEquippedWeaponTag = InWeaponTag;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
		if (AWarriorWeaponBase* const* Found = CharacterCarriedWeaponMap.Find(InWeaponTag))
			return *Found;
	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid()) return nullptr;
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* Weapon = GetCurrentEquippedWeapon();
		check(Weapon);

		Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision );
		if (!bShouldEnable) OverlappedActors.Empty();
	}

	//TODO: Handle Bodycollision boxes
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
