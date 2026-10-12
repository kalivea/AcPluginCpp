#pragma once
#include "PolaCustomPillar.h"
#define POLA_DLL_PILLAR_TOOLS
#include "PillarToolsDef.h"

class POLADLL_PILLAR_TOOLS_API  PillarTools
{
public:
	static Acad::ErrorStatus DetectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter, bool& found);
	static Acad::ErrorStatus DetectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter, bool& found);
	static Acad::ErrorStatus GetAllPillarChecked(AcDbObjectIdArray& all_pillar_ids);
	static Acad::ErrorStatus SaveAllPillarInfoToFileChecked(const std::wstring& filePath);
	// Borrows pillar_t. A failed or unmatched read leaves its custom fields unchanged.
	static Acad::ErrorStatus LoadPillarInfoFromFileBySnChecked(const std::wstring& filePath, const TCHAR* sn, CPolaCustomPillar* pillar_t, bool& found);
	static bool detectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter);
	static bool detectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter);
	static bool GetAllPillar(AcDbObjectIdArray& all_pillar_ids);
	static bool SaveAllPillarInfoToFile(const std::wstring& filePath);
	static bool LoadPillarInfoFromFileBySn(const std::wstring& filePath, const TCHAR* sn, CPolaCustomPillar* pillar_t);
	static bool IsPillarInfoInFile(const std::wstring& filePath, const TCHAR* sn);
	static bool GetPillarBinPath(CString& binPath);
};
