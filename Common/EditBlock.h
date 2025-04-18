#pragma once
#include "StdAfx.h"

class POLADLL_COMMON_API EditBlock
{
public:

	static bool IsBlockExist(const TCHAR* block_name, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
	static AcDbObjectId GetBlockId(const TCHAR* block_name, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
	static bool SetBlockInsertPointToCenter(AcDbObjectId blockDefId);
	static AcDbObjectId InsertBlockRef(AcDbObjectId block_def_id, const AcGePoint3d& insert_point, double scale, double rotation);
	static void SetScale(AcDbBlockReference* block_ref, double scale);
};

