#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

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
