#include "ZGP0Rules.h"

namespace
{
    bool ValidElement(EZGElement E) { return static_cast<uint8>(E) < 5; }
    bool ValidBody(EZGBody B) { return static_cast<uint8>(B) < 3; }
    float SafeP(float P) { return FMath::IsFinite(P) ? FMath::Clamp(P, -1.f, 1.f) : 0.f; }
}
float UZGP0Rules::SpeedMultiplier(float P)
{
    P = SafeP(P); return P < 0.f ? 1.f + .8f * P : 1.f + .5f * P;
}
float UZGP0Rules::DamageMultiplier(float P)
{
    P = SafeP(P); return P < 0.f ? 1.f + .5f * P : 1.f + P;
}
float UZGP0Rules::ConversionBeta(EZGElement Attack, EZGElement Target)
{
    static constexpr float Table[5][5] = {
        {1,1.5f,.75f,.5f,1.25f}, {.5f,1,1.25f,.75f,1.5f},
        {1.25f,.75f,1,1.5f,.5f}, {1.5f,1.25f,.5f,1,.75f},
        {.75f,.5f,1.5f,1.25f,1}};
    if (!ValidElement(Attack) || !ValidElement(Target)) return 1.f;
    return Table[static_cast<uint8>(Attack)][static_cast<uint8>(Target)];
}
float UZGP0Rules::MixPolarity(float OldP, float Ce, float Pb, float Cb, float Beta)
{
    if (!FMath::IsFinite(Ce) || Ce <= 0 || !FMath::IsFinite(Cb) || Cb < 0 ||
        !FMath::IsFinite(Beta) || Beta < 0) return SafeP(OldP);
    const double Effective = static_cast<double>(Cb) * Beta;
    // Weighted-capacity interpretation of the document's numeric example.
    return SafeP(static_cast<float>((Ce * static_cast<double>(SafeP(OldP)) + Effective * SafeP(Pb)) / (Ce + Effective)));
}
float UZGP0Rules::EnvironmentStep(float P, float G) { return SafeP(SafeP(P) + .05f * (SafeP(G) - SafeP(P))); }
float UZGP0Rules::TowerPolarity(float BaseP, float G) { return SafeP(SafeP(BaseP) + SafeP(G)); }
float UZGP0Rules::SeasonG(int32 Season)
{
    static constexpr float Values[] = {.2f,.5f,-.2f,-.5f};
    return Values[FMath::Clamp(Season,0,3)];
}
FZGHitResult UZGP0Rules::ApplyHit(FZGEnemyState& Enemy, const FZGAttackSnapshot& Attack)
{
    FZGHitResult R; R.OldPolarity = R.NewPolarity = Enemy.Polarity;
    if (Enemy.Terminal != EZGTerminal::Alive || !FMath::IsFinite(Enemy.Health) || Enemy.Health <= 0 ||
        !FMath::IsFinite(Enemy.Capacity) || Enemy.Capacity <= 0 ||
        !FMath::IsFinite(Enemy.Polarity) || !FMath::IsFinite(Attack.Polarity) ||
        !FMath::IsFinite(Attack.BaseDamage) || Attack.BaseDamage < 0 ||
        !FMath::IsFinite(Attack.RawCapacity) || Attack.RawCapacity < 0 ||
        !ValidElement(Enemy.Element) || !ValidElement(Attack.Element)) return R;
    R.bApplied = true;
    R.Beta = ConversionBeta(Attack.Element, Enemy.Element);
    const double Damage = static_cast<double>(Attack.BaseDamage) * DamageMultiplier(Enemy.Polarity);
    R.AppliedDamage = static_cast<float>(FMath::Min(static_cast<double>(Enemy.Health), Damage));
    Enemy.Health -= R.AppliedDamage;
    if (Enemy.Health <= 0)
    {
        R.bKilledNow = TryTerminate(Enemy, EZGTerminal::Killed);
        return R; // Dead targets do not receive polarity conversion.
    }
    Enemy.Polarity = MixPolarity(Enemy.Polarity, Enemy.Capacity, Attack.Polarity, Attack.RawCapacity, R.Beta);
    R.NewPolarity = Enemy.Polarity;
    return R;
}
bool UZGP0Rules::TryTerminate(FZGEnemyState& Enemy, EZGTerminal Reason)
{
    if (Enemy.Terminal != EZGTerminal::Alive || Reason == EZGTerminal::Alive || static_cast<uint8>(Reason) > 3) return false;
    Enemy.Terminal = Reason; return true;
}
FZGAttackSnapshot UZGP0Rules::MakeSecondary(const FZGAttackSnapshot& Parent, float Scale, int32 NewAttackId)
{
    FZGAttackSnapshot R = Parent;
    const float S = FMath::IsFinite(Scale) ? FMath::Clamp(Scale, 0.f, 1.f) : 0.f;
    R.AttackId = NewAttackId; R.BaseDamage *= S; R.RawCapacity *= S;
    R.Body = EZGBody::None; R.bAllowSecondary = false;
    return R;
}
int32 UZGP0Rules::BodyCost(EZGBody Body)
{
    switch (Body) { case EZGBody::None: return 0; case EZGBody::Split: return 60; case EZGBody::Residue: return 70; default: return -1; }
}
int32 UZGP0Rules::ToneCost(int32 OldIndex, int32 NewIndex)
{
    if (OldIndex < 0 || OldIndex > 4 || NewIndex < 0 || NewIndex > 4) return -1;
    return FMath::Abs(NewIndex - OldIndex) * 5;
}
float UZGP0Rules::TonePolarity(int32 Index)
{
    static constexpr float Values[] = {-1.f,-.75f,0,.75f,1.f};
    return Values[FMath::Clamp(Index,0,4)];
}
int32 UZGP0Rules::BuildCost(EZGElement Base, int32 ToneIndex, EZGBody Body)
{
    const int32 Adjustment = ToneCost(2,ToneIndex);
    if (!ValidElement(Base) || !ValidBody(Body) || Adjustment < 0) return -1;
    return TowerDefaults(Base).BaseCost + 20 + Adjustment + BodyCost(Body);
}
int32 UZGP0Rules::DemolishRefund(EZGElement Base, EZGBody Body)
{
    if (!ValidElement(Base) || !ValidBody(Body)) return -1;
    return (TowerDefaults(Base).BaseCost + 20 + BodyCost(Body)) * 3 / 5;
}
bool UZGP0Rules::TryChangeBody(int32& Qi, EZGBody& Current, EZGBody Requested)
{
    if (Qi < 0 || !ValidBody(Current) || !ValidBody(Requested)) return false;
    if (Current == Requested) return true;
    const int64 NewQi = static_cast<int64>(Qi) + BodyCost(Current) * 3 / 5 - BodyCost(Requested);
    if (NewQi < 0 || NewQi > MAX_int32) return false;
    Qi = static_cast<int32>(NewQi); Current = Requested; return true;
}
FZGEnemyRow UZGP0Rules::EnemyDefaults(EZGEnemyKind Kind)
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
FZGTowerRow UZGP0Rules::TowerDefaults(EZGElement Base)
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
TArray<FZGSpawnPlan> UZGP0Rules::BuildDefaultSpawnPlan()
{
    // Counts ordered B,F,T,L. This function is a baseline fixture, not an asset loader.
    static constexpr int32 Counts[16][4] = {
        {4,0,0,0},{3,2,0,0},{3,0,2,0},{3,2,0,1},
        {3,3,0,0},{4,2,1,0},{4,2,1,1},{4,3,1,1},
        {4,2,2,1},{4,2,3,1},{4,3,2,1},{4,3,2,2},
        {4,3,2,2},{4,3,3,2},{5,3,3,2},{5,4,3,2}};
    static constexpr int32 Scan[] = {2,0,1,3};
    static constexpr int32 Cycle[] = {0,1,0,0,2,0,3,0,0,4};
    static constexpr float Intervals[] = {1.5f,1.2f,1.f,.8f};
    static constexpr EZGElement Elements[4][5] = {
        {EZGElement::Wood,EZGElement::Metal,EZGElement::Water,EZGElement::Fire,EZGElement::Earth},
        {EZGElement::Fire,EZGElement::Metal,EZGElement::Wood,EZGElement::Water,EZGElement::Earth},
        {EZGElement::Metal,EZGElement::Wood,EZGElement::Water,EZGElement::Fire,EZGElement::Earth},
        {EZGElement::Water,EZGElement::Metal,EZGElement::Wood,EZGElement::Fire,EZGElement::Earth}};
    TArray<FZGSpawnPlan> Out; Out.Reserve(140);
    int32 GlobalB = 0;
    for (int32 Season=0; Season<4; ++Season)
    {
        int32 SeasonOrdinal=0;
        for (int32 Wave=0; Wave<4; ++Wave)
        {
            int32 Remaining[4]; int32 Total=0;
            for (int32 I=0; I<4; ++I) { Remaining[I]=Counts[Season*4+Wave][I]; Total+=Remaining[I]; }
            int32 Ordinal=0;
            while (Ordinal<Total)
            {
                for (int32 K : Scan)
                {
                    if (Remaining[K] <= 0) continue;
                    --Remaining[K];
                    FZGSpawnPlan P; P.Season=Season; P.Wave=Wave; P.Entrance=Wave;
                    P.TimeSeconds=Season*60.f+Wave*15.f+Ordinal*Intervals[Season];
                    P.Kind=static_cast<EZGEnemyKind>(K);
                    P.Element=Elements[Season][Cycle[SeasonOrdinal%10]];
                    P.InitialPolarity=EnemyDefaults(P.Kind).InitialPolarity;
                    if (P.Kind==EZGEnemyKind::B) { ++GlobalB; P.InitialPolarity=(GlobalB%2==1)?-.5f:.5f; }
                    Out.Add(P); ++Ordinal; ++SeasonOrdinal;
                }
            }
        }
    }
    return Out;
}
EZGPhase UZGP0Rules::EvaluatePhase(float TimeSeconds, int32 Spawned, int32 Planned, int32 Alive, int32 BellHealth)
{
    if (BellHealth <= 0) return EZGPhase::Lost;
    if (TimeSeconds < 0) return EZGPhase::Preparation;
    if (TimeSeconds >= 240 && Planned >= 0 && Spawned == Planned && Alive == 0) return EZGPhase::Won;
    return TimeSeconds >= 240 ? EZGPhase::Cleanup : EZGPhase::Combat;
}
bool UZGP0Rules::SegmentCircle(const FVector2D& Start, const FVector2D& End, const FVector2D& Center, float Radius, float& OutFraction)
{
    OutFraction = 0;
    if (!FMath::IsFinite(Radius) || Radius < 0 || Start.ContainsNaN() || End.ContainsNaN() || Center.ContainsNaN()) return false;
    const FVector2D D=End-Start, F=Start-Center;
    const double C=F.SizeSquared()-static_cast<double>(Radius)*Radius;
    if (C <= 0) return true;
    const double A=D.SizeSquared();
    if (A <= UE_SMALL_NUMBER) return false;
    const double B=2.0*FVector2D::DotProduct(F,D), Disc=B*B-4*A*C;
    if (Disc < 0) return false;
    const double T=(-B-FMath::Sqrt(Disc))/(2*A);
    if (T < 0 || T > 1) return false;
    OutFraction=static_cast<float>(T); return true;
}
