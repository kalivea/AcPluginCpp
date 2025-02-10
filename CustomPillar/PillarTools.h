#pragma once
#include "PolaCustomPillar.h"
#define POLA_DLL_PILLAR_TOOLS
#include "PillarToolsDef.h"

class POLADLL_PILLAR_TOOLS_API  PillarTools
{
public:
    static bool detectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter);
};

