#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"

class UHeroUIComponent;

UCLASS(Abstract, Blueprintable)
class WARRIOR_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnOwningHeroUIComponentInitialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* OwningHeroUIComponent);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEnemyUIComponentInitialized"))
	void BP_OnOwningEnemyComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);
	
public:
	UFUNCTION(BLueprintCallable)
	void InitEnemyCreateWidget(AActor* OwningEnemyActor);
};
