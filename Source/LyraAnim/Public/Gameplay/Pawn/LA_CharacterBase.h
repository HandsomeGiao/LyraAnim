#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LA_CharacterBase.generated.h"

UCLASS()
class LYRAANIM_API ALA_CharacterBase : public ACharacter
{
  GENERATED_BODY()

public:
  ALA_CharacterBase();

  /** Moves relative to the controller's yaw. X is right/left and Y is forward/backward. */
  virtual void Move(const FVector2D& MovementInput);

  /** Applies controller yaw and pitch input. */
  virtual void Look(const FVector2D& LookInput);

  virtual void StartJump();
  virtual void StopJump();
  virtual void ToggleCrouch();
};
