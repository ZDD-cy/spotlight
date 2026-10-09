#pragma once
#include "ZGCombatTypes.h"

namespace ZhongGu { namespace P0 { namespace Session
{
    EZGPhase EvaluatePhase(float TimeSeconds, int32 Spawned, int32 Planned, int32 Alive, int32 BellHealth);
}}}
