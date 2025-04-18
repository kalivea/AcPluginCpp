#pragma once
#include "StdAfx.h"
#include "rxmfcapi.h"
class POLADLL_TOOLS_API EditDwg
{
public:
	static AcDbObjectId CopyBlockDefFromOtherDwg(const TCHAR* file_name, const TCHAR* block_def_name);
	static AcDbObjectId InsertDwgAsBlockDef(const TCHAR* file_name, const TCHAR* block_name, bool overwrite, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
};
