#pragma once
#include "PolaCustomPillar.h"
#define POLA_DLL_PILLAR_TOOLS
#include "PillarToolsDef.h"

class POLADLL_PILLAR_TOOLS_API  PillarTools
{
public:
	static bool detectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter);
	static bool detectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter);
	static bool GetAllPillar(AcDbObjectIdArray& all_pillar_ids);
	static bool SaveAllPillarInfoToFile(const std::wstring& filePath);
	static bool LoadPillarInfoFromFileBySn(const std::wstring& filePath, TCHAR* sn, CPolaCustomPillar* pillar_t);
	static bool IsPillarInfoInFile(const std::wstring& filePath, TCHAR* sn);
	static bool GetPillarBinPath(CString& binPath);
};

