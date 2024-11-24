#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

class UWarriorAbilitySystemComponent;

UENUM()
enum class EWarriorConfirmType:uint8
{
	Yes, No	
};

UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "WarriorFunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag InTag);
	
	UFUNCTION(BlueprintCallable, Category = "WarriorFunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag InTag);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "WarriorFunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InTag, EWarriorConfirmType& OutConfirmType);
};
