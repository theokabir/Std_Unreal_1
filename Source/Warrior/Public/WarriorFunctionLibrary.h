#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorFunctionLibrary.generated.h"

struct FScalableFloat;
struct FGameplayTag;
class UPawnCombatComponent;
class UWarriorAbilitySystemComponent;

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

	static UPawnCombatComponent* NativeGetCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "WarriorFunctionLibrary", meta = (DisplayName = "Get CombatComponent by Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetCombatComponentByActor(AActor* InActor,EWarriorValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "WarriorFunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "WarriorFunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& ScalableFloat, float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "WarriorFunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InTarget, float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "WarriorFunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);
};
