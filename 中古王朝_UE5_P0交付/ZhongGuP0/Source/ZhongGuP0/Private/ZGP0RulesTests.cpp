#include "ZGP0Rules.h"
#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZGCoreTest, "ZhongGu.P0.Core", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FZGCoreTest::RunTest(const FString& Parameters)
{
    TestTrue(TEXT("Yin speed"), FMath::IsNearlyEqual(UZGP0Rules::SpeedMultiplier(-1),.2f));
    TestTrue(TEXT("Yang damage"), FMath::IsNearlyEqual(UZGP0Rules::DamageMultiplier(1),2.f));
    TestTrue(TEXT("Capacity example beta 2"),FMath::IsNearlyEqual(UZGP0Rules::MixPolarity(.5f,100,-.75f,50,2),-.125f));
    TestTrue(TEXT("Environment"),FMath::IsNearlyEqual(UZGP0Rules::EnvironmentStep(-.6f,.2f),-.56f));
    TestTrue(TEXT("Summer tower"),FMath::IsNearlyEqual(UZGP0Rules::TowerPolarity(-.75f,.5f),-.25f));
    FZGEnemyState E; E.Health=200; E.Capacity=100; E.Polarity=.5f; E.Element=EZGElement::Earth;
    FZGAttackSnapshot A; A.BaseDamage=50; A.RawCapacity=50; A.Polarity=-.75f;
    const auto Hit=UZGP0Rules::ApplyHit(E,A);
    TestTrue(TEXT("Damage uses old p"),FMath::IsNearlyEqual(E.Health,125.f));
    TestTrue(TEXT("Capacity fixed"),FMath::IsNearlyEqual(E.Capacity,100.f));
    TestTrue(TEXT("Weighted conversion beta 1"),FMath::IsNearlyEqual(E.Polarity,1.f/12.f));
    E.Health=1; E.Polarity=.5f;
    TestTrue(TEXT("First kill"),UZGP0Rules::ApplyHit(E,A).bKilledNow);
    TestFalse(TEXT("No second kill"),UZGP0Rules::ApplyHit(E,A).bKilledNow);
    TestFalse(TEXT("Dead cannot leak"),UZGP0Rules::TryTerminate(E,EZGTerminal::Leaked));
    TestTrue(TEXT("Death skips conversion"),FMath::IsNearlyEqual(E.Polarity,.5f));
    auto Child=UZGP0Rules::MakeSecondary(A,.4f,2);
    TestFalse(TEXT("No recursive body"),Child.bAllowSecondary);
    TestTrue(TEXT("Snapshot unchanged"),FMath::IsNearlyEqual(A.BaseDamage,50.f));
    TestEqual(TEXT("Fire build"),UZGP0Rules::BuildCost(EZGElement::Fire,3,EZGBody::Split),185);
    TestEqual(TEXT("Earth refund"),UZGP0Rules::DemolishRefund(EZGElement::Earth,EZGBody::Split),84);
    int32 Qi=33; EZGBody Body=EZGBody::Split;
    TestFalse(TEXT("Insufficient body exchange"),UZGP0Rules::TryChangeBody(Qi,Body,EZGBody::Residue));
    TestEqual(TEXT("Atomic qi"),Qi,33); TestTrue(TEXT("Atomic body"),Body==EZGBody::Split);
    Qi=34; TestTrue(TEXT("Exchange with refund"),UZGP0Rules::TryChangeBody(Qi,Body,EZGBody::Residue));
    TestEqual(TEXT("Exchange cost"),Qi,0);
    float T=0;
    TestTrue(TEXT("Bell swept contact"),UZGP0Rules::SegmentCircle(FVector2D(200,0),FVector2D(-200,0),FVector2D::ZeroVector,100,T));
    TestTrue(TEXT("Earliest contact"),FMath::IsNearlyEqual(T,.25f));
    TestTrue(TEXT("Failure priority"),UZGP0Rules::EvaluatePhase(240,140,140,0,0)==EZGPhase::Lost);
    TestTrue(TEXT("Cleanup"),UZGP0Rules::EvaluatePhase(241,140,140,1,10)==EZGPhase::Cleanup);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZGWaveTest, "ZhongGu.P0.Waves", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FZGWaveTest::RunTest(const FString& Parameters)
{
    const auto Plan=UZGP0Rules::BuildDefaultSpawnPlan();
    TestEqual(TEXT("140 enemies"),Plan.Num(),140);
    int32 Counts[4]={}, Seasons[4]={}, Reward=0, B=0, Elements[4][5]={};
    float Previous=-1;
    for (const auto& P : Plan)
    {
        ++Counts[static_cast<uint8>(P.Kind)]; ++Seasons[P.Season]; ++Elements[P.Season][static_cast<uint8>(P.Element)];
        Reward+=UZGP0Rules::EnemyDefaults(P.Kind).Reward;
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
