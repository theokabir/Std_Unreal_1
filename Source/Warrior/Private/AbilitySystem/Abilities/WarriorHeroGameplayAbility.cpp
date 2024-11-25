#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "Items/Weapons/WarriorWeaponBase.h"

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

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
	int32 InUsedComboCount)
{
	check(EffectClass);
	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);
	
	if (InCurrentAttackTypeTag.IsValid())
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	
	return EffectSpecHandle;
}

bool UWarriorHeroGameplayAbility::AttachToBone(const FName Bone, const FGameplayTag InWeaponTag)
{

	const FAttachmentTransformRules Rules(
	EAttachmentRule::SnapToTarget,
	EAttachmentRule::KeepRelative,
	EAttachmentRule::KeepWorld,
	true);

	USkeletalMeshComponent* Parent = GetActorInfo().SkeletalMeshComponent.Get();
	
	return GetHeroCombatComponentFromActorInfo()->GetCharacterCarriedWeaponByTag(InWeaponTag)->AttachToComponent(Parent, Rules, Bone);
}
