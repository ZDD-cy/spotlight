#pragma once
#include "ZGCombatTypes.h"

namespace ZhongGu { namespace P0 { namespace Polarity
{
    float SpeedMultiplier(float P);
    float DamageMultiplier(float P);
    float ConversionBeta(EZGElement Attack, EZGElement Target);
    float MixPolarity(float OldP, float Ce, float Pb, float Cb, float Beta);
    float EnvironmentStep(float P, float G);
    float TowerPolarity(float BaseP, float G);
}}}
