#include "LyraAnim/Public/Gameplay/Pawn/LA_CharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"

ALA_CharacterBase::ALA_CharacterBase()
{
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
  MovementComponent->bOrientRotationToMovement = true;
  MovementComponent->RotationRate = FRotator(0.0, 500.0, 0.0);
  MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ALA_CharacterBase::Move(const FVector2D& MovementInput)
{
  if (!Controller)
  {
    return;
  }

  const FRotator ControlRotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0, ControlRotation.Yaw, 0.0);

  const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  AddMovementInput(ForwardDirection, MovementInput.Y);
  AddMovementInput(RightDirection, MovementInput.X);
}

void ALA_CharacterBase::Look(const FVector2D& LookInput)
{
  AddControllerYawInput(LookInput.X);
  AddControllerPitchInput(LookInput.Y);
}

void ALA_CharacterBase::StartJump()
{
  Jump();
}

void ALA_CharacterBase::StopJump()
{
  StopJumping();
}

void ALA_CharacterBase::ToggleCrouch()
{
  bIsCrouched ? UnCrouch() : Crouch();
}
