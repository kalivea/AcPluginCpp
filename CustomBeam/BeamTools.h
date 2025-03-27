#pragma once
#include "PolaCustomBeam.h"
#include "PillarTools.h"
#include "PolaCustomPillar.h"
#define POLA_DLL_BEAM_TOOLS
#include "BeamToolsDef.h"

class POLADLL_BEAM_TOOLS_API BeamTools
{
public:
	static bool IsPointInsideBeam(const CPolaCustomBeam* beam, const AcGePoint3d& point, const AcGeTol& tol = AcGeContext::gTol);
	static bool GetBeamSegmentMidPoint(const CPolaCustomBeam* beam, AcGePoint3dArray& beam_seg_mid);
	static bool GetAllPillarCenterInBeam(const CPolaCustomBeam* beam, AcGePoint3dArray& pillar_center);
	static bool GetAllPillarInBeam(const CPolaCustomBeam* beam, AcDbObjectIdArray& pillar_ids);
private:
	static bool IsPointOnLine(const AcGePoint3d& point, const AcGePoint3d& line_start, const AcGePoint3d& line_end, const AcGeTol& tol);
};