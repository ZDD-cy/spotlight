#include "ZGGameplayRuleLibrary.h"
#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZGWaveScheduleTest, "ZhongGu.Rules.Waves", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FZGWaveScheduleTest::RunTest(const FString& Parameters)
{
    const auto Plan=UZGGameplayRuleLibrary::BuildDefaultSpawnPlan();
    TestEqual(TEXT("140 enemies"),Plan.Num(),140);
    int32 Counts[4]={}, Seasons[4]={}, Reward=0, B=0, Elements[4][5]={};
    float Previous=-1;
    for (const auto& P : Plan)
    {
        ++Counts[static_cast<uint8>(P.Kind)]; ++Seasons[P.Season]; ++Elements[P.Season][static_cast<uint8>(P.Element)];
        Reward+=UZGGameplayRuleLibrary::EnemyDefaults(P.Kind).Reward;
        TestTrue(TEXT("Chronological"),P.TimeSeconds>=Previous); Previous=P.TimeSeconds;
        TestTrue(TEXT("No season overflow"),P.TimeSeconds<(P.Season+1)*60.f);
        if (P.Kind==EZGEnemyKind::B) { ++B; TestTrue(TEXT("Global B polarity"),FMath::IsNearlyEqual(P.InitialPolarity,B%2?-.5f:.5f)); }
    }
    TestEqual(TEXT("B total"),Counts[0],62); TestEqual(TEXT("F total"),Counts[1],37);
    TestEqual(TEXT("T total"),Counts[2],25); TestEqual(TEXT("L total"),Counts[3],16);
    TestEqual(TEXT("Reward total"),Reward,1429);
    const int32 Main[]={1,3,0,2};
    for(int32 S=0; S<4; ++S)
    {
        TestEqual(TEXT("Season count"),Seasons[S],20+10*S);
        for(int32 E=0; E<5; ++E) TestEqual(TEXT("Season element ratio"),Elements[S][E],E==Main[S]?(12+6*S):(2+S));
    }
    return true;
}
#endif
