#pragma once
#include "gepnt3d.h"
#include "acadstrc.h"
#include "InteractiveDef.h"
class POLADLL_INTERACTIVE_API CoordinateSystem
{
public:
    static Acad::ErrorStatus WcsToUcs(const AcGePoint3d& input, AcGePoint3d& output);
    static Acad::ErrorStatus UcsToWcs(const AcGePoint3d& input, AcGePoint3d& output);
	static AcGePoint3d WcsToUcs(const AcGePoint3d& wcs_point);
	static AcGePoint3d UcsToWcs(const AcGePoint3d& ucs_point);
};
