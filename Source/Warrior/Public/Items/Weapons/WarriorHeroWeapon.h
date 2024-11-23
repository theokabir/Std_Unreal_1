#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "WarriorHeroWeapon.generated.h"
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& Handles);

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;


private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
