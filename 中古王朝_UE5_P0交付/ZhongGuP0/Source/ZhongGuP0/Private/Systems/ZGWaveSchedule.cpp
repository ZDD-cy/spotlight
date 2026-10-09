#include "Systems/ZGWaveSchedule.h"
#include "Systems/ZGPrototypeConfig.h"

namespace ZhongGu { namespace P0 { namespace Waves
{
TArray<FZGSpawnPlan> BuildDefaultSpawnPlan()
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
                    P.InitialPolarity=Config::EnemyDefaults(P.Kind).InitialPolarity;
                    if (P.Kind==EZGEnemyKind::B) { ++GlobalB; P.InitialPolarity=(GlobalB%2==1)?-.5f:.5f; }
                    Out.Add(P); ++Ordinal; ++SeasonOrdinal;
                }
            }
        }
    }
    return Out;
}
}}}
