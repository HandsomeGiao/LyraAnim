#include "LA_GameInst.h"
#include "JsEnv.h"

void ULA_GameInst::Init()
{
  Super::Init();

  JsEnv = MakeShared<puerts::FJsEnv>();
  JsEnv->Start(TEXT("GameStartup"));
}

void ULA_GameInst::Shutdown()
{
  Super::Shutdown();

  JsEnv.Reset();
}
