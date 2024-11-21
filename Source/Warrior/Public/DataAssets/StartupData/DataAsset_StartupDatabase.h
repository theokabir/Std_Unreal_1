// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartupDatabase.generated.h"

class UWarriorAbilitySystemComponent;
class UWarriorGameplayAbility;
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_StartupDatabase : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray< TSubclassOf <UWarriorGameplayAbility> > ActivateOnGivenAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray< TSubclassOf <UWarriorGameplayAbility> > ReactiveAbilities;

	virtual void GrantAbilities(const TArray< TSubclassOf <UWarriorGameplayAbility> >& InAbilitiesToGive, UWarriorAbilitySystemComponent* InWarriorASCToGive, int32 ApplyLevel = 1);

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InWarriorASCToGive, int32 ApplyLevel = 1);
};
