#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZGCombatTypes.h"
#include "ZGGameplayRuleLibrary.generated.h"

// Stateless functions: state ownership, event dispatch and Actor lifetime belong to the caller.
UCLASS()
class ZHONGGUP0_API UZGGameplayRuleLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float SpeedMultiplier(float P);
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float DamageMultiplier(float P);
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float ConversionBeta(EZGElement Attack, EZGElement Target);
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float MixPolarity(float OldP, float Ce, float Pb, float Cb, float Beta);
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float EnvironmentStep(float P, float G);
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float TowerPolarity(float BaseP, float G);
    UFUNCTION(BlueprintPure, Category="ZG|Rules") static float SeasonG(int32 Season);
    // Mutates only the supplied state. Reward once iff bKilledNow is true.
    UFUNCTION(BlueprintCallable, Category="ZG|Combat") static FZGHitResult ApplyHit(UPARAM(ref) FZGEnemyState& Enemy, const FZGAttackSnapshot& Attack);
    UFUNCTION(BlueprintCallable, Category="ZG|Combat") static bool TryTerminate(UPARAM(ref) FZGEnemyState& Enemy, EZGTerminal Reason);
    UFUNCTION(BlueprintPure, Category="ZG|Combat") static FZGAttackSnapshot MakeSecondary(const FZGAttackSnapshot& Parent, float Scale, int32 NewAttackId);
    UFUNCTION(BlueprintPure, Category="ZG|Economy") static int32 BodyCost(EZGBody Body);
    UFUNCTION(BlueprintPure, Category="ZG|Economy") static int32 ToneCost(int32 OldIndex, int32 NewIndex);
    UFUNCTION(BlueprintPure, Category="ZG|Economy") static float TonePolarity(int32 Index);
    UFUNCTION(BlueprintPure, Category="ZG|Economy") static int32 BuildCost(EZGElement Base, int32 ToneIndex, EZGBody Body);
    UFUNCTION(BlueprintPure, Category="ZG|Economy") static int32 DemolishRefund(EZGElement Base, EZGBody Body);
    // Caller clears old residue only after success. Cooling state is not changed here.
    UFUNCTION(BlueprintCallable, Category="ZG|Economy") static bool TryChangeBody(UPARAM(ref) int32& Qi, UPARAM(ref) EZGBody& Current, EZGBody Requested);
    UFUNCTION(BlueprintPure, Category="ZG|Config") static FZGEnemyRow EnemyDefaults(EZGEnemyKind Kind);
    UFUNCTION(BlueprintPure, Category="ZG|Config") static FZGTowerRow TowerDefaults(EZGElement Base);
    UFUNCTION(BlueprintPure, Category="ZG|Waves") static TArray<FZGSpawnPlan> BuildDefaultSpawnPlan();
    UFUNCTION(BlueprintPure, Category="ZG|Session") static EZGPhase EvaluatePhase(float TimeSeconds, int32 Spawned, int32 Planned, int32 Alive, int32 BellHealth);
    // XY segment vs circle; returns earliest contact fraction [0,1].
    UFUNCTION(BlueprintPure, Category="ZG|Geometry") static bool SegmentCircle(const FVector2D& Start, const FVector2D& End, const FVector2D& Center, float Radius, float& OutFraction);
};
