#pragma once
#include "StdAfx.h"
#include "rxmfcapi.h"
class POLADLL_TOOLS_API DwgTools
{
public:
	static bool GetAcadPath(CString& acad_path);
	static bool SaveDwgFiles(AcDbDatabase* database, CString& save_path, CString& dwg_name);
	static AcDbExtents GetModelExtent(AcDbDatabase* database);
	static void ZoomDwgExtent(AcDbDatabase* database);

	static AcDbObjectId CopyBlockDefFromOtherDwg(const TCHAR* file_name, const TCHAR* block_def_name);
};

