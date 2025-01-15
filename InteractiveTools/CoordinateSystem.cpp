#include "stdafx.h"
#include "CoordinateSystem.h"

AcGePoint3d CoordinateSystem::WcsToUcs(const AcGePoint3d& wcs_point)
{
	AcGePoint3d ucs_point;
	struct resbuf result_buff_from, result_buff_to;
	result_buff_from.restype = RTSHORT;
	result_buff_from.resval.rint = 0;
	result_buff_to.restype = RTSHORT;
	result_buff_to.resval.rint = 1;

	acedTrans(asDblArray(wcs_point), &result_buff_from, &result_buff_to, 0, asDblArray(ucs_point));
	return ucs_point;
}

AcGePoint3d CoordinateSystem::UcsToWcs(const AcGePoint3d& ucs_point)
{
	AcGePoint3d wcs_point;
	struct resbuf result_buff_from, result_buff_to;
	result_buff_from.restype = RTSHORT;
	result_buff_from.resval.rint = 1;
	result_buff_to.restype = RTSHORT;
	result_buff_to.resval.rint = 0;

	acedTrans(asDblArray(ucs_point), &result_buff_from, &result_buff_to, 0, asDblArray(wcs_point));
	return wcs_point;
}