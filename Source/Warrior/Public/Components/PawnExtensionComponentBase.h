// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIOR_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template Parameter to get GetPawn must derife from APawn");
		return CastChecked<T>(GetOwner());
	};

	APawn* GetOwningPawn() const
	{
		return CastChecked<APawn>(GetOwner());
	}

	template<class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' template Parameter to get GetOwningController must derive from AController");
		return GetOwningController<APawn>()->GetController<T>();
	}
	
};
