#pragma once
#include "ZGCombatTypes.h"
namespace ZhongGu { namespace P0 { namespace Validation
{
    inline bool ValidElement(EZGElement E) { return static_cast<uint8>(E) < 5; }
    inline bool ValidBody(EZGBody B) { return static_cast<uint8>(B) < 3; }
    inline float SafeP(float P) { return FMath::IsFinite(P) ? FMath::Clamp(P, -1.f, 1.f) : 0.f; }
}}}
