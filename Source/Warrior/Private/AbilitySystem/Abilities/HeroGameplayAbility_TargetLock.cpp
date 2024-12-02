#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Character/WarriorHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Controllers/WarriorHeroController.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Cleanup();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();
	if (AvailableTargetToLock.IsEmpty()) { CancelTargetLockAbility(); return; }
	CurrentLockedActor = GetNearestTargetFromAvailableTarget(AvailableTargetToLock);

	if (CurrentLockedActor)
	{
		Debug::Print(CurrentLockedActor->GetActorNameOrLabel());
		DrawTargetLockWidget();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AWarriorHeroCharacter* Character = GetHeroCharacterFromActorInfo();
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		Character,
		Character->GetActorLocation(),
		Character->GetActorLocation() + Character->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		Character->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowDebug? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		HitResults,
		true,
		FColor::Red
	);

	for (const FHitResult& Hits : HitResults)
	{
		if (AActor* HitActor = Hits.GetActor())
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableTargetToLock.AddUnique(HitActor);
				Debug::Print(HitActor->GetActorNameOrLabel());
			}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableTarget(const TArray<AActor*>& InAvailableActors)
{
	float Distance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, Distance);
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{

	if (DrawnTargetWidget) return;
	
	checkf(TargetWidgetClass, TEXT("Forgot to assign a widgetClass"));
	DrawnTargetWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetWidgetClass);
	check(DrawnTargetWidget);

	DrawnTargetWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::Cleanup()
{
	AvailableTargetToLock.Empty();
	CurrentLockedActor = nullptr;
	if (DrawnTargetWidget) DrawnTargetWidget->RemoveFromParent();
}
