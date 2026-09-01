#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LA_PCBase.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class LYRAANIM_API ALA_PCBase : public APlayerController
{
  GENERATED_BODY()

protected:
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
  virtual void SetupInputComponent() override;

  // ====================== 增强输入 =======================
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
  TObjectPtr<UInputMappingContext> DefaultMappingContext;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
  TObjectPtr<UInputAction> MoveAction;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
  TObjectPtr<UInputAction> LookAction;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
  TObjectPtr<UInputAction> JumpAction;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
  TObjectPtr<UInputAction> CrouchAction;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
  int32 MappingContextPriority = 0;
  // ~增强输入

private:
  void AddDefaultMappingContext();
  void RemoveDefaultMappingContext();

  void Move(const FInputActionValue& Value);
  void Look(const FInputActionValue& Value);
  void StartJump();
  void StopJump();
  void ToggleCrouch();
};
