#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZGP0Rules.generated.h"

UENUM(BlueprintType)
enum class EZGElement : uint8 { Metal, Wood, Water, Fire, Earth };
UENUM(BlueprintType)
enum class EZGBody : uint8 { None, Split, Residue };
UENUM(BlueprintType)
enum class EZGEnemyKind : uint8 { B, F, T, L };
UENUM(BlueprintType)
enum class EZGTerminal : uint8 { Alive, Killed, Leaked, Removed };
UENUM(BlueprintType)
enum class EZGPhase : uint8 { Preparation, Combat, Cleanup, Won, Lost };

USTRUCT(BlueprintType)
struct ZHONGGUP0_API FZGEnemyState
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StableId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Health = 90.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Capacity = 60.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Polarity = -0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EZGElement Element = EZGElement::Wood;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EZGTerminal Terminal = EZGTerminal::Alive;
};

USTRUCT(BlueprintType)
struct ZHONGGUP0_API FZGAttackSnapshot
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SourceId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AttackId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EZGElement Element = EZGElement::Earth;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EZGBody Body = EZGBody::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Polarity = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseDamage = 30.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float RawCapacity = 30.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bAllowSecondary = true;
};

USTRUCT(BlueprintType)
struct ZHONGGUP0_API FZGHitResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) bool bApplied = false;
    UPROPERTY(BlueprintReadOnly) bool bKilledNow = false;
    UPROPERTY(BlueprintReadOnly) float AppliedDamage = 0.f;
    UPROPERTY(BlueprintReadOnly) float Beta = 1.f;
    UPROPERTY(BlueprintReadOnly) float OldPolarity = 0.f;
    UPROPERTY(BlueprintReadOnly) float NewPolarity = 0.f;
};

USTRUCT(BlueprintType)
struct ZHONGGUP0_API FZGEnemyRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Health = 90.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseSpeed = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float InitialPolarity = -0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Capacity = 60.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Reward = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BellDamage = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MeleeDamage = 6.f;
};

USTRUCT(BlueprintType)
struct ZHONGGUP0_API FZGTowerRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseDamage = 30.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Interval = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Range = 650.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BaseCost = 60;
};

USTRUCT(BlueprintType)
struct ZHONGGUP0_API FZGSpawnPlan
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) int32 Season = 0;
    UPROPERTY(BlueprintReadOnly) int32 Wave = 0;
    // 0 E(+X), 1 S(-Y), 2 W(-X), 3 N(+Y).
    UPROPERTY(BlueprintReadOnly) int32 Entrance = 0;
    UPROPERTY(BlueprintReadOnly) float TimeSeconds = 0.f;
    UPROPERTY(BlueprintReadOnly) EZGEnemyKind Kind = EZGEnemyKind::B;
    UPROPERTY(BlueprintReadOnly) EZGElement Element = EZGElement::Wood;
    UPROPERTY(BlueprintReadOnly) float InitialPolarity = -0.5f;
};

// Stateless functions: state ownership, event dispatch and Actor lifetime belong to the caller.
UCLASS()
class ZHONGGUP0_API UZGP0Rules : public UBlueprintFunctionLibrary
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
