#pragma once
#include "ZGCombatTypes.h"

namespace ZhongGu { namespace P0 { namespace Economy
{
    int32 BodyCost(EZGBody Body);
    int32 ToneCost(int32 OldIndex, int32 NewIndex);
    int32 BuildCost(EZGElement Base, int32 ToneIndex, EZGBody Body);
    int32 DemolishRefund(EZGElement Base, EZGBody Body);
    bool TryChangeBody(int32& Qi, EZGBody& Current, EZGBody Requested);
}}}
