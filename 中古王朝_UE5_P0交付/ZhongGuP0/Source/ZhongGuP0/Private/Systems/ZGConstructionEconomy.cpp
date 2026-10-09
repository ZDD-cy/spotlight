#include "Systems/ZGConstructionEconomy.h"
#include "Systems/ZGPrototypeConfig.h"
#include "Systems/ZGRuleValidation.h"

namespace ZhongGu { namespace P0 { namespace Economy
{
int32 BodyCost(EZGBody Body)
{
    switch (Body) { case EZGBody::None: return 0; case EZGBody::Split: return 60; case EZGBody::Residue: return 70; default: return -1; }
}
int32 ToneCost(int32 OldIndex, int32 NewIndex)
{
    if (OldIndex < 0 || OldIndex > 4 || NewIndex < 0 || NewIndex > 4) return -1;
    return FMath::Abs(NewIndex - OldIndex) * 5;
}
int32 BuildCost(EZGElement Base, int32 ToneIndex, EZGBody Body)
{
    const int32 Adjustment = ToneCost(2,ToneIndex);
    if (!Validation::ValidElement(Base) || !Validation::ValidBody(Body) || Adjustment < 0) return -1;
    return Config::TowerDefaults(Base).BaseCost + 20 + Adjustment + BodyCost(Body);
}
int32 DemolishRefund(EZGElement Base, EZGBody Body)
{
    if (!Validation::ValidElement(Base) || !Validation::ValidBody(Body)) return -1;
    return (Config::TowerDefaults(Base).BaseCost + 20 + BodyCost(Body)) * 3 / 5;
}
bool TryChangeBody(int32& Qi, EZGBody& Current, EZGBody Requested)
{
    if (Qi < 0 || !Validation::ValidBody(Current) || !Validation::ValidBody(Requested)) return false;
    if (Current == Requested) return true;
    const int64 NewQi = static_cast<int64>(Qi) + BodyCost(Current) * 3 / 5 - BodyCost(Requested);
    if (NewQi < 0 || NewQi > MAX_int32) return false;
    Qi = static_cast<int32>(NewQi); Current = Requested; return true;
}
}}}
