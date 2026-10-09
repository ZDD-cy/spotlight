#include "Systems/ZGYinYangMath.h"
#include "Systems/ZGRuleValidation.h"

namespace ZhongGu { namespace P0 { namespace Polarity
{
float SpeedMultiplier(float P)
{
    P = Validation::SafeP(P); return P < 0.f ? 1.f + .8f * P : 1.f + .5f * P;
}
float DamageMultiplier(float P)
{
    P = Validation::SafeP(P); return P < 0.f ? 1.f + .5f * P : 1.f + P;
}
float ConversionBeta(EZGElement Attack, EZGElement Target)
{
    static constexpr float Table[5][5] = {
        {1,1.5f,.75f,.5f,1.25f}, {.5f,1,1.25f,.75f,1.5f},
        {1.25f,.75f,1,1.5f,.5f}, {1.5f,1.25f,.5f,1,.75f},
        {.75f,.5f,1.5f,1.25f,1}};
    if (!Validation::ValidElement(Attack) || !Validation::ValidElement(Target)) return 1.f;
    return Table[static_cast<uint8>(Attack)][static_cast<uint8>(Target)];
}
float MixPolarity(float OldP, float Ce, float Pb, float Cb, float Beta)
{
    if (!FMath::IsFinite(Ce) || Ce <= 0 || !FMath::IsFinite(Cb) || Cb < 0 ||
        !FMath::IsFinite(Beta) || Beta < 0) return Validation::SafeP(OldP);
    const double Effective = static_cast<double>(Cb) * Beta;
    // Weighted-capacity interpretation of the document's numeric example.
    return Validation::SafeP(static_cast<float>((Ce * static_cast<double>(Validation::SafeP(OldP)) + Effective * Validation::SafeP(Pb)) / (Ce + Effective)));
}
float EnvironmentStep(float P, float G)
{ return Validation::SafeP(Validation::SafeP(P) + .05f * (Validation::SafeP(G) - Validation::SafeP(P))); }
float TowerPolarity(float BaseP, float G)
{ return Validation::SafeP(Validation::SafeP(BaseP) + Validation::SafeP(G)); }
}}}
