#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Character/WarriorHeroCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Controllers/WarriorHeroController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitMappingContext();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	RemoveMappingContext();
	Cleanup();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, WarriorGameplayTags::Shared_Status_Death) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Death))
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation =
		!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling)
		&& !UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		// Orient Character
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		LookAtRot -= FRotator(TargetLockCameraOffSet, 0, 0);

		const FRotator CurrentControllRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControllRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();
	TArray<AActor*> ActorsOnLeft, ActorsOnRight;
	AActor* NewTargetToLock = nullptr;
	GetAvailableActorsAroundTheTarget(ActorsOnLeft, ActorsOnRight);

	NewTargetToLock  = (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
	? GetNearestTargetFromAvailableTarget(ActorsOnLeft)
	: GetNearestTargetFromAvailableTarget(ActorsOnRight);

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
	
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
		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableTargetToLock.Empty();
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
			}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableTarget(const TArray<AActor*>& InAvailableActors)
{
	float Distance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, Distance);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTheTarget(TArray<AActor*>& OutActorsOnLeft,
	TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableTargetToLock.IsEmpty()) { CancelTargetLockAbility(); return; }

	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableTargetToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
		const FVector CrossProduct = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossProduct.Z > 0.f) OutActorsOnRight.AddUnique(AvailableActor);
		else OutActorsOnLeft.AddUnique(AvailableActor);
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{

	if (DrawnTargetWidget) return;
	
	checkf(TargetWidgetClass, TEXT("Forgot to assign a widgetClass"));
	DrawnTargetWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetWidgetClass);
	check(DrawnTargetWidget);

	DrawnTargetWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetWidget || !CurrentLockedActor) { CancelTargetLockAbility(); return; }

	FVector2D ScreenPosition;
	
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
		DrawnTargetWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (const USizeBox* SizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = SizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = SizeBox->GetHeightOverride();
				}
			}
		);

	ScreenPosition -= TargetLockWidgetSize / 2.f;
	
	DrawnTargetWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetHeroControllerFromActorInfo()->GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::RemoveMappingContext()
{
	if (!GetHeroControllerFromActorInfo()) return;
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
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
	DrawnTargetWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;
	CachedDefaultMaxWalkSpeed = 0.f;
}
