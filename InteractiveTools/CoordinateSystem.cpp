#include "stdafx.h"
#include "CoordinateSystem.h"
#include <cmath>

namespace {
Acad::ErrorStatus TransformCoordinate(const AcGePoint3d& input, short from, short to, AcGePoint3d& output) {
    const AcGePoint3d source = input;
    output = AcGePoint3d::kOrigin;
    if (!std::isfinite(source.x) || !std::isfinite(source.y) || !std::isfinite(source.z))
        return Acad::eInvalidInput;
    resbuf origin = {}, target = {};
    origin.restype = target.restype = RTSHORT;
    origin.resval.rint = from;
    target.resval.rint = to;
    AcGePoint3d converted;
    if (acedTrans(asDblArray(source), &origin, &target, 0, asDblArray(converted)) != RTNORM)
        return Acad::eInvalidInput;
    if (!std::isfinite(converted.x) || !std::isfinite(converted.y) || !std::isfinite(converted.z))
        return Acad::eInvalidInput;
    output = converted;
    return Acad::eOk;
}
void ReportCoordinateError(Acad::ErrorStatus status) {
    if (status != Acad::eOk) acutPrintf(_T("\nCoordinate transform failed: %d\n"), static_cast<int>(status));
}
}
Acad::ErrorStatus CoordinateSystem::WcsToUcs(const AcGePoint3d& input, AcGePoint3d& output) {
    return TransformCoordinate(input, 0, 1, output);
}
Acad::ErrorStatus CoordinateSystem::UcsToWcs(const AcGePoint3d& input, AcGePoint3d& output) {
    return TransformCoordinate(input, 1, 0, output);
}
AcGePoint3d CoordinateSystem::WcsToUcs(const AcGePoint3d& input) {
    AcGePoint3d output; ReportCoordinateError(WcsToUcs(input, output)); return output;
}
AcGePoint3d CoordinateSystem::UcsToWcs(const AcGePoint3d& input) {
    AcGePoint3d output; ReportCoordinateError(UcsToWcs(input, output)); return output;
}
