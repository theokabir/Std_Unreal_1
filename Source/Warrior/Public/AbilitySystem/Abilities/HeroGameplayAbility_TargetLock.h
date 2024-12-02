// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestTargetFromAvailableTarget(const TArray<AActor*>& InAvailableActors);
	void DrawTargetLockWidget();
	
	void CancelTargetLockAbility();
	void Cleanup();

	UPROPERTY(EditDefaultsOnly, Category="TargetLock")
	float BoxTraceDistance = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="TargetLock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category="TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;
	
	UPROPERTY(EditDefaultsOnly, Category="TargetLock")
	bool bShowDebug = false;

	UPROPERTY()
	TArray<AActor*> AvailableTargetToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY(EditDefaultsOnly, Category="TargetLock")
	TSubclassOf<UWarriorWidgetBase> TargetWidgetClass;

	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetWidget;
};
