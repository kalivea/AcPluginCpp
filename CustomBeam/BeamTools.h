#pragma once
#include "PolaCustomBeam.h"
#include "PolaCustomPillar.h"
#define POLA_DLL_BEAM_TOOLS
#include "BeamToolsDef.h"

class POLADLL_BEAM_TOOLS_API BeamTools
{
public:
	static bool IsPointInsideBeam(const CPolaCustomBeam* beam, const AcGePoint3d& point, const AcGeTol& tol = AcGeContext::gTol);
private:
	static bool IsPointOnLine(const AcGePoint3d& point, const AcGePoint3d& line_start, const AcGePoint3d& line_end, const AcGeTol& tol);
};


