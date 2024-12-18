#include "Character/WarriorHeroCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WarriorGameplayTags.h" 
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/WarriorInputComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartupData/DataAsset_StartupDatabase.h"
#include "GameFramework/SpringArmComponent.h"
#include "DebugHelper.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AWarriorHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AWarriorHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AWarriorHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartupDatabase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
		}
	}
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfig, TEXT("InputComponent Invalido"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	checkf(Subsystem, TEXT("Subsystem Invalido"));

	Subsystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);

	UWarriorInputComponent* PlayerComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);

	PlayerComponent->BindNativeInputAction(InputConfig, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	PlayerComponent->BindNativeInputAction(InputConfig, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);

	PlayerComponent->BindNativeInputAction(InputConfig, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::SwitchTargetTriggered);
	PlayerComponent->BindNativeInputAction(InputConfig, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::SwitchTargetCompleted);
	
	PlayerComponent->BindAbilityInputAction(InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AWarriorHeroCharacter::Move(const FInputActionValue& Val)
{
	const FVector2d MovementDirection = Val.Get<FVector2d>();
	const FRotator MovementRotation(0.f,  Controller->GetControlRotation().Yaw, 0.f);

	if (MovementDirection.Y != 0.f)
	{
		const FVector Forward = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(Forward, MovementDirection.Y);
	}

	if (MovementDirection.X != 0.f)
	{
		const FVector Right = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(Right, MovementDirection.X);
	}
}

void AWarriorHeroCharacter::Look(const FInputActionValue& Val)
{
	const FVector2d LookVector = Val.Get<FVector2d>();
	if (LookVector.X != 0.f) AddControllerYawInput(LookVector.X);
	if (LookVector.Y != 0.f) AddControllerPitchInput(-LookVector.Y);
}

void AWarriorHeroCharacter::SwitchTargetTriggered(const FInputActionValue& Val)
{
	SwitchDirection = Val.Get<FVector2d>();
}

void AWarriorHeroCharacter::SwitchTargetCompleted(const FInputActionValue& Val)
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		(SwitchDirection.X >0.f)?WarriorGameplayTags::Player_Event_SwitchTarget_Right:WarriorGameplayTags::Player_Event_SwitchTarget_Left,
		Data
		);
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
