#include "Systems/ZGMatchOutcome.h"

namespace ZhongGu { namespace P0 { namespace Session
{
EZGPhase EvaluatePhase(float TimeSeconds, int32 Spawned, int32 Planned, int32 Alive, int32 BellHealth)
{
    if (BellHealth <= 0) return EZGPhase::Lost;
    if (TimeSeconds < 0) return EZGPhase::Preparation;
    if (TimeSeconds >= 240 && Planned >= 0 && Spawned == Planned && Alive == 0) return EZGPhase::Won;
    return TimeSeconds >= 240 ? EZGPhase::Cleanup : EZGPhase::Combat;
}
}}}
