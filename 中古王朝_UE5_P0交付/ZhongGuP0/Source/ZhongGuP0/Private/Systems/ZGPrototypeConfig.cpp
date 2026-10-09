#include "Systems/ZGPrototypeConfig.h"

namespace ZhongGu { namespace P0 { namespace Config
{
float SeasonG(int32 Season)
{
    static constexpr float Values[] = {.2f,.5f,-.2f,-.5f};
    return Values[FMath::Clamp(Season,0,3)];
}
float TonePolarity(int32 Index)
{
    static constexpr float Values[] = {-1.f,-.75f,0,.75f,1.f};
    return Values[FMath::Clamp(Index,0,4)];
}
FZGEnemyRow EnemyDefaults(EZGEnemyKind Kind)
{
    FZGEnemyRow R;
    switch (Kind)
    {
    case EZGEnemyKind::F: R.Health=55; R.BaseSpeed=145; R.InitialPolarity=.5f; R.Capacity=35; R.Reward=7; R.BellDamage=4; R.MeleeDamage=5; break;
    case EZGEnemyKind::T: R.Health=240; R.BaseSpeed=65; R.InitialPolarity=-.75f; R.Capacity=160; R.Reward=18; R.BellDamage=12; R.MeleeDamage=16; break;
    case EZGEnemyKind::L: R.Health=150; R.BaseSpeed=90; R.InitialPolarity=.75f; R.Capacity=100; R.Reward=14; R.BellDamage=8; R.MeleeDamage=10; break;
    default: break;
    }
    return R;
}
FZGTowerRow TowerDefaults(EZGElement Base)
{
    FZGTowerRow R;
    switch (Base)
    {
    case EZGElement::Metal: R.BaseDamage=24; R.Interval=1.2f; R.Range=700; R.BaseCost=90; break;
    case EZGElement::Wood: R.BaseDamage=18; R.Interval=.9f; R.Range=80; R.BaseCost=90; break;
    case EZGElement::Water: R.BaseDamage=8; R.Interval=.5f; R.Range=600; R.BaseCost=110; break;
    case EZGElement::Fire: R.BaseDamage=30; R.Interval=1.8f; R.Range=650; R.BaseCost=100; break;
    default: break;
    }
    return R;
}
}}}
