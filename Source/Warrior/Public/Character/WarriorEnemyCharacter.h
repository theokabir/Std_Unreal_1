#pragma once

#include "CoreMinimal.h"
#include "Character/WarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorEnemyCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorEnemyCharacter();
	FORCEINLINE UEnemyCombatComponent* GetCombatComponent() const { return EnemyCombatComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEnemyCombatComponent* EnemyCombatComponent;
	
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

private:
	void InitEnemyStartupData();
	
};
