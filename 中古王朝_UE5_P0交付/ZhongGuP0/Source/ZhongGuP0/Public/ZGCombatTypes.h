#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZGCombatTypes.generated.h"

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
