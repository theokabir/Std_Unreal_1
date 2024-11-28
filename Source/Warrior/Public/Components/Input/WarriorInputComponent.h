#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"

UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(
		const UDataAsset_InputConfig* DataAsset_InputConfig,
		const FGameplayTag& InputTag,
		ETriggerEvent TriggerEvent,
		UserObject* Object,
		CallbackFunc Func
	);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(
		const UDataAsset_InputConfig* DataAsset_InputConfig,
		UserObject* Object,
		CallbackFunc PressedFunc,
		CallbackFunc ReleasedFunc
	);
	
};

template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindNativeInputAction(
	const UDataAsset_InputConfig* DataAsset_InputConfig,
	const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent,
	UserObject* Object,
	CallbackFunc Func)
{
	checkf(DataAsset_InputConfig, TEXT("impossível realizar bind sem o devido data asset atribuido"));
	if (UInputAction* FouncAdction = DataAsset_InputConfig->FindInputActionByTag(InputTag))
	{
		BindAction(FouncAdction, TriggerEvent, Object, Func);
	}
}

template <class UserObject, typename CallbackFunc>
void UWarriorInputComponent::BindAbilityInputAction(
	const UDataAsset_InputConfig* DataAsset_InputConfig,
	UserObject* Object,
	CallbackFunc PressedFunc,
	CallbackFunc ReleasedFunc)
{
	checkf(DataAsset_InputConfig, TEXT("impossível realizar bind sem o devido data asset atribuido"));
	for (const FWarriorInputActionConfig& AbilityInputActionConfig: DataAsset_InputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid()) continue;
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, Object, PressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
