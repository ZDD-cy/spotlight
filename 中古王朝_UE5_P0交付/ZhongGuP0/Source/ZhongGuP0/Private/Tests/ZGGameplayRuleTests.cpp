#include "ZGGameplayRuleLibrary.h"
#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZGGameplayRulesTest, "ZhongGu.Rules.Core", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FZGGameplayRulesTest::RunTest(const FString& Parameters)
{
    TestTrue(TEXT("Yin speed"), FMath::IsNearlyEqual(UZGGameplayRuleLibrary::SpeedMultiplier(-1),.2f));
    TestTrue(TEXT("Yang damage"), FMath::IsNearlyEqual(UZGGameplayRuleLibrary::DamageMultiplier(1),2.f));
    TestTrue(TEXT("Capacity example beta 2"),FMath::IsNearlyEqual(UZGGameplayRuleLibrary::MixPolarity(.5f,100,-.75f,50,2),-.125f));
    TestTrue(TEXT("Environment"),FMath::IsNearlyEqual(UZGGameplayRuleLibrary::EnvironmentStep(-.6f,.2f),-.56f));
    TestTrue(TEXT("Summer tower"),FMath::IsNearlyEqual(UZGGameplayRuleLibrary::TowerPolarity(-.75f,.5f),-.25f));
    FZGEnemyState E; E.Health=200; E.Capacity=100; E.Polarity=.5f; E.Element=EZGElement::Earth;
    FZGAttackSnapshot A; A.BaseDamage=50; A.RawCapacity=50; A.Polarity=-.75f;
    const auto Hit=UZGGameplayRuleLibrary::ApplyHit(E,A);
    TestTrue(TEXT("Damage uses old p"),FMath::IsNearlyEqual(E.Health,125.f));
    TestTrue(TEXT("Capacity fixed"),FMath::IsNearlyEqual(E.Capacity,100.f));
    TestTrue(TEXT("Weighted conversion beta 1"),FMath::IsNearlyEqual(E.Polarity,1.f/12.f));
    E.Health=1; E.Polarity=.5f;
    TestTrue(TEXT("First kill"),UZGGameplayRuleLibrary::ApplyHit(E,A).bKilledNow);
    TestFalse(TEXT("No second kill"),UZGGameplayRuleLibrary::ApplyHit(E,A).bKilledNow);
    TestFalse(TEXT("Dead cannot leak"),UZGGameplayRuleLibrary::TryTerminate(E,EZGTerminal::Leaked));
    TestTrue(TEXT("Death skips conversion"),FMath::IsNearlyEqual(E.Polarity,.5f));
    auto Child=UZGGameplayRuleLibrary::MakeSecondary(A,.4f,2);
    TestFalse(TEXT("No recursive body"),Child.bAllowSecondary);
    TestTrue(TEXT("Snapshot unchanged"),FMath::IsNearlyEqual(A.BaseDamage,50.f));
    TestEqual(TEXT("Fire build"),UZGGameplayRuleLibrary::BuildCost(EZGElement::Fire,3,EZGBody::Split),185);
    TestEqual(TEXT("Earth refund"),UZGGameplayRuleLibrary::DemolishRefund(EZGElement::Earth,EZGBody::Split),84);
    int32 Qi=33; EZGBody Body=EZGBody::Split;
    TestFalse(TEXT("Insufficient body exchange"),UZGGameplayRuleLibrary::TryChangeBody(Qi,Body,EZGBody::Residue));
    TestEqual(TEXT("Atomic qi"),Qi,33); TestTrue(TEXT("Atomic body"),Body==EZGBody::Split);
    Qi=34; TestTrue(TEXT("Exchange with refund"),UZGGameplayRuleLibrary::TryChangeBody(Qi,Body,EZGBody::Residue));
    TestEqual(TEXT("Exchange cost"),Qi,0);
    float T=0;
    TestTrue(TEXT("Bell swept contact"),UZGGameplayRuleLibrary::SegmentCircle(FVector2D(200,0),FVector2D(-200,0),FVector2D::ZeroVector,100,T));
    TestTrue(TEXT("Earliest contact"),FMath::IsNearlyEqual(T,.25f));
    TestTrue(TEXT("Failure priority"),UZGGameplayRuleLibrary::EvaluatePhase(240,140,140,0,0)==EZGPhase::Lost);
    TestTrue(TEXT("Cleanup"),UZGGameplayRuleLibrary::EvaluatePhase(241,140,140,1,10)==EZGPhase::Cleanup);
    return true;
}
#endif
