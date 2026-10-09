#pragma once
#include "ZGCombatTypes.h"

namespace ZhongGu { namespace P0 { namespace Config
{
    float SeasonG(int32 Season);
    float TonePolarity(int32 Index);
    FZGEnemyRow EnemyDefaults(EZGEnemyKind Kind);
    FZGTowerRow TowerDefaults(EZGElement Base);
}}}
