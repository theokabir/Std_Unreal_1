#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

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
