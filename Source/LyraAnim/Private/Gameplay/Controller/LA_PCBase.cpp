#include "LyraAnim/Public/Gameplay/Controller/LA_PCBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "LyraAnim/Public/Gameplay/Pawn/LA_CharacterBase.h"

void ALA_PCBase::BeginPlay()
{
  Super::BeginPlay();

  AddDefaultMappingContext();
}

void ALA_PCBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  RemoveDefaultMappingContext();

  Super::EndPlay(EndPlayReason);
}

void ALA_PCBase::SetupInputComponent()
{
  Super::SetupInputComponent();

  UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
  if (!ensureMsgf(EnhancedInputComponent, TEXT("ALA_PCBase requires an Enhanced Input Component.")))
  {
    return;
  }

  if (MoveAction)
  {
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
  }

  if (LookAction)
  {
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
  }

  if (JumpAction)
  {
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::StartJump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ThisClass::StopJump);
  }

  if (CrouchAction)
  {
    EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::ToggleCrouch);
  }
}

void ALA_PCBase::AddDefaultMappingContext()
{
  if (!IsLocalController() || !DefaultMappingContext)
  {
    return;
  }

  if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
  {
    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
          LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
      InputSubsystem->AddMappingContext(DefaultMappingContext, MappingContextPriority);
    }
  }
}

void ALA_PCBase::RemoveDefaultMappingContext()
{
  if (!DefaultMappingContext)
  {
    return;
  }

  if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
  {
    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
          LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
      InputSubsystem->RemoveMappingContext(DefaultMappingContext);
    }
  }
}

void ALA_PCBase::Move(const FInputActionValue& Value)
{
  if (ALA_CharacterBase* ControlledCharacter = GetPawn<ALA_CharacterBase>())
  {
    ControlledCharacter->Move(Value.Get<FVector2D>());
  }
}

void ALA_PCBase::Look(const FInputActionValue& Value)
{
  if (ALA_CharacterBase* ControlledCharacter = GetPawn<ALA_CharacterBase>())
  {
    ControlledCharacter->Look(Value.Get<FVector2D>());
  }
}

void ALA_PCBase::StartJump()
{
  if (ALA_CharacterBase* ControlledCharacter = GetPawn<ALA_CharacterBase>())
  {
    ControlledCharacter->StartJump();
  }
}

void ALA_PCBase::StopJump()
{
  if (ALA_CharacterBase* ControlledCharacter = GetPawn<ALA_CharacterBase>())
  {
    ControlledCharacter->StopJump();
  }
}

void ALA_PCBase::ToggleCrouch()
{
  if (ALA_CharacterBase* ControlledCharacter = GetPawn<ALA_CharacterBase>())
  {
    ControlledCharacter->ToggleCrouch();
  }
}
