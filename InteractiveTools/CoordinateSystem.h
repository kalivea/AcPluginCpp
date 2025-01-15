#pragma once
class POLADLL_INTERACTIVE_API CoordinateSystem
{
public:
	static AcGePoint3d WcsToUcs(const AcGePoint3d& wcs_point);
	static AcGePoint3d UcsToWcs(const AcGePoint3d& ucs_point);
};

