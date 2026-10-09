#include "ZGGameplayRuleLibrary.h"
#include "Systems/ZGYinYangMath.h"
#include "Systems/ZGDamageResolver.h"
#include "Systems/ZGConstructionEconomy.h"
#include "Systems/ZGPrototypeConfig.h"
#include "Systems/ZGWaveSchedule.h"
#include "Systems/ZGMatchOutcome.h"
#include "Systems/ZGSweptCollision.h"

// Blueprint-facing entry point; gameplay calculations live in dedicated systems.
float UZGGameplayRuleLibrary::SpeedMultiplier(float P)
{
    return ZhongGu::P0::Polarity::SpeedMultiplier(P);
}
float UZGGameplayRuleLibrary::DamageMultiplier(float P)
{
    return ZhongGu::P0::Polarity::DamageMultiplier(P);
}
float UZGGameplayRuleLibrary::ConversionBeta(EZGElement Attack, EZGElement Target)
{
    return ZhongGu::P0::Polarity::ConversionBeta(Attack, Target);
}
float UZGGameplayRuleLibrary::MixPolarity(float OldP, float Ce, float Pb, float Cb, float Beta)
{
    return ZhongGu::P0::Polarity::MixPolarity(OldP, Ce, Pb, Cb, Beta);
}
float UZGGameplayRuleLibrary::EnvironmentStep(float P, float G)
{
    return ZhongGu::P0::Polarity::EnvironmentStep(P, G);
}
float UZGGameplayRuleLibrary::TowerPolarity(float BaseP, float G)
{
    return ZhongGu::P0::Polarity::TowerPolarity(BaseP, G);
}
float UZGGameplayRuleLibrary::SeasonG(int32 Season)
{
    return ZhongGu::P0::Config::SeasonG(Season);
}
FZGHitResult UZGGameplayRuleLibrary::ApplyHit(FZGEnemyState& Enemy, const FZGAttackSnapshot& Attack)
{
    return ZhongGu::P0::Combat::ApplyHit(Enemy, Attack);
}
bool UZGGameplayRuleLibrary::TryTerminate(FZGEnemyState& Enemy, EZGTerminal Reason)
{
    return ZhongGu::P0::Combat::TryTerminate(Enemy, Reason);
}
FZGAttackSnapshot UZGGameplayRuleLibrary::MakeSecondary(const FZGAttackSnapshot& Parent, float Scale, int32 NewAttackId)
{
    return ZhongGu::P0::Combat::MakeSecondary(Parent, Scale, NewAttackId);
}
int32 UZGGameplayRuleLibrary::BodyCost(EZGBody Body)
{
    return ZhongGu::P0::Economy::BodyCost(Body);
}
int32 UZGGameplayRuleLibrary::ToneCost(int32 OldIndex, int32 NewIndex)
{
    return ZhongGu::P0::Economy::ToneCost(OldIndex, NewIndex);
}
float UZGGameplayRuleLibrary::TonePolarity(int32 Index)
{
    return ZhongGu::P0::Config::TonePolarity(Index);
}
int32 UZGGameplayRuleLibrary::BuildCost(EZGElement Base, int32 ToneIndex, EZGBody Body)
{
    return ZhongGu::P0::Economy::BuildCost(Base, ToneIndex, Body);
}
int32 UZGGameplayRuleLibrary::DemolishRefund(EZGElement Base, EZGBody Body)
{
    return ZhongGu::P0::Economy::DemolishRefund(Base, Body);
}
bool UZGGameplayRuleLibrary::TryChangeBody(int32& Qi, EZGBody& Current, EZGBody Requested)
{
    return ZhongGu::P0::Economy::TryChangeBody(Qi, Current, Requested);
}
FZGEnemyRow UZGGameplayRuleLibrary::EnemyDefaults(EZGEnemyKind Kind)
{
    return ZhongGu::P0::Config::EnemyDefaults(Kind);
}
FZGTowerRow UZGGameplayRuleLibrary::TowerDefaults(EZGElement Base)
{
    return ZhongGu::P0::Config::TowerDefaults(Base);
}
TArray<FZGSpawnPlan> UZGGameplayRuleLibrary::BuildDefaultSpawnPlan()
{
    return ZhongGu::P0::Waves::BuildDefaultSpawnPlan();
}
EZGPhase UZGGameplayRuleLibrary::EvaluatePhase(float TimeSeconds, int32 Spawned, int32 Planned, int32 Alive, int32 BellHealth)
{
    return ZhongGu::P0::Session::EvaluatePhase(TimeSeconds, Spawned, Planned, Alive, BellHealth);
}
bool UZGGameplayRuleLibrary::SegmentCircle(const FVector2D& Start, const FVector2D& End, const FVector2D& Center, float Radius, float& OutFraction)
{
    return ZhongGu::P0::Geometry::SegmentCircle(Start, End, Center, Radius, OutFraction);
}
