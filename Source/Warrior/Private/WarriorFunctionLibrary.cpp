#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	checkf(InActor, TEXT("InActor variable not specified"));
	UAbilitySystemComponent* RawComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	return CastChecked<UWarriorAbilitySystemComponent>(RawComponent);
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag InTag)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(InTag)) ASC->AddLooseGameplayTag(InTag);
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag InTag)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(InTag)) ASC->RemoveLooseGameplayTag(InTag);
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag InTag)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(InTag);
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InTag,
	EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, InTag) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetCombatComponentFromActor(AActor* InActor)
{
	checkf(InActor, TEXT("InActor variable not specified"));
	if (const IPawnCombatInterface* CombatInterface = Cast<IPawnCombatInterface>(InActor))
		return CombatInterface->GetPawnCombatComponent();

	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetCombatComponentByActor(AActor* InActor,
	EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* Component = NativeGetCombatComponentFromActor(InActor);
	OutValidType = Component ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	return Component;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	if (!(QueryTeamAgent && TargetTeamAgent)) return false;
	return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
}

float UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& ScalableFloat, float InLevel)
{
	return ScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UWarriorFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InTarget,
	float& OutAngleDifference)
{
	check(InAttacker && InTarget);

	const FVector TargetForward = InTarget->GetActorForwardVector();
	const FVector TargetToAttacker = (InAttacker->GetActorForwardVector() - InTarget->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(TargetForward, TargetToAttacker);
	OutAngleDifference =  UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(TargetForward, TargetToAttacker);

	if (CrossResult.Z < 0.f) OutAngleDifference *= -1.f;

	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f) return WarriorGameplayTags::Shared_Status_HitReact_Front;
	if (OutAngleDifference >= -135.f && OutAngleDifference <= -45.f) return WarriorGameplayTags::Shared_Status_HitReact_Right;
	if (OutAngleDifference >= 45.f && OutAngleDifference <= 135.f) return WarriorGameplayTags::Shared_Status_HitReact_Left;
	if (OutAngleDifference >= 135 || OutAngleDifference <= 135.f) return WarriorGameplayTags::Shared_Status_HitReact_Back;
	
	return WarriorGameplayTags::Shared_Status_HitReact_Front;
}

bool UWarriorFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	return DotResult < .1f;
}
