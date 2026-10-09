#pragma once
#include "ZGCombatTypes.h"

namespace ZhongGu { namespace P0 { namespace Combat
{
    FZGHitResult ApplyHit(FZGEnemyState& Enemy, const FZGAttackSnapshot& Attack);
    bool TryTerminate(FZGEnemyState& Enemy, EZGTerminal Reason);
    FZGAttackSnapshot MakeSecondary(const FZGAttackSnapshot& Parent, float Scale, int32 NewAttackId);
}}}
