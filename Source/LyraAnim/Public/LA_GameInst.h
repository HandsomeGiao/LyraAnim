#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LA_GameInst.generated.h"

namespace puerts
{
  class FJsEnv;
}

UCLASS()
class LYRAANIM_API ULA_GameInst : public UGameInstance
{
  GENERATED_BODY()

public:
  virtual void Init() override;
  virtual void Shutdown() override;

private:
  TSharedPtr<puerts::FJsEnv> JsEnv;
};