#pragma once
#include "ZGCombatTypes.h"

namespace ZhongGu { namespace P0 { namespace Geometry
{
    bool SegmentCircle(const FVector2D& Start, const FVector2D& End, const FVector2D& Center, float Radius, float& OutFraction);
}}}
