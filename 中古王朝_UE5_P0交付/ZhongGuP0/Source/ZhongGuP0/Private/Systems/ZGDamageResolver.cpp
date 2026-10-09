#include "Systems/ZGDamageResolver.h"
#include "Systems/ZGYinYangMath.h"
#include "Systems/ZGRuleValidation.h"

namespace ZhongGu { namespace P0 { namespace Combat
{
FZGHitResult ApplyHit(FZGEnemyState& Enemy, const FZGAttackSnapshot& Attack)
{
    FZGHitResult R; R.OldPolarity = R.NewPolarity = Enemy.Polarity;
    if (Enemy.Terminal != EZGTerminal::Alive || !FMath::IsFinite(Enemy.Health) || Enemy.Health <= 0 ||
        !FMath::IsFinite(Enemy.Capacity) || Enemy.Capacity <= 0 ||
        !FMath::IsFinite(Enemy.Polarity) || !FMath::IsFinite(Attack.Polarity) ||
        !FMath::IsFinite(Attack.BaseDamage) || Attack.BaseDamage < 0 ||
        !FMath::IsFinite(Attack.RawCapacity) || Attack.RawCapacity < 0 ||
        !Validation::ValidElement(Enemy.Element) || !Validation::ValidElement(Attack.Element)) return R;
    R.bApplied = true;
    R.Beta = Polarity::ConversionBeta(Attack.Element, Enemy.Element);
    const double Damage = static_cast<double>(Attack.BaseDamage) * Polarity::DamageMultiplier(Enemy.Polarity);
    R.AppliedDamage = static_cast<float>(FMath::Min(static_cast<double>(Enemy.Health), Damage));
    Enemy.Health -= R.AppliedDamage;
    if (Enemy.Health <= 0)
    {
        R.bKilledNow = TryTerminate(Enemy, EZGTerminal::Killed);
        return R; // Dead targets do not receive polarity conversion.
    }
    Enemy.Polarity = Polarity::MixPolarity(Enemy.Polarity, Enemy.Capacity, Attack.Polarity, Attack.RawCapacity, R.Beta);
    R.NewPolarity = Enemy.Polarity;
    return R;
}
bool TryTerminate(FZGEnemyState& Enemy, EZGTerminal Reason)
{
    if (Enemy.Terminal != EZGTerminal::Alive || Reason == EZGTerminal::Alive || static_cast<uint8>(Reason) > 3) return false;
    Enemy.Terminal = Reason; return true;
}
FZGAttackSnapshot MakeSecondary(const FZGAttackSnapshot& Parent, float Scale, int32 NewAttackId)
{
    FZGAttackSnapshot R = Parent;
    const float S = FMath::IsFinite(Scale) ? FMath::Clamp(Scale, 0.f, 1.f) : 0.f;
    R.AttackId = NewAttackId; R.BaseDamage *= S; R.RawCapacity *= S;
    R.Body = EZGBody::None; R.bAllowSecondary = false;
    return R;
}
}}}
