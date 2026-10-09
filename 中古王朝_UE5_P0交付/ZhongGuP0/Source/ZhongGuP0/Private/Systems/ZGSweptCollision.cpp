#include "Systems/ZGSweptCollision.h"

namespace ZhongGu { namespace P0 { namespace Geometry
{
bool SegmentCircle(const FVector2D& Start, const FVector2D& End, const FVector2D& Center, float Radius, float& OutFraction)
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
}}}
