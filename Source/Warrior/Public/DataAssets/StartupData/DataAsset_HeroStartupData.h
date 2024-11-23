// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAssets/StartupData/DataAsset_StartupDatabase.h"
#include "DataAsset_HeroStartupData.generated.h"

struct FWarriorHeroAbilitySet;
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_HeroStartupData : public UDataAsset_StartupDatabase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InWarriorASCToGive, int32 ApplyLevel = 1) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InpuitTag"))
	TArray< FWarriorHeroAbilitySet > HeroStartAbilitySets;
	
};
