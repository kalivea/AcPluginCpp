#pragma once
#include "dbmain.h"
#include "dbapserv.h"
#include <tchar.h>
#include "CommonDef.h"

class POLADLL_COMMON_API EditDwg
{
public:
    static Acad::ErrorStatus CopyBlockDefFromOtherDwg(const TCHAR* file_name, const TCHAR* block_def_name, AcDbObjectId& id);
    static AcDbObjectId CopyBlockDefFromOtherDwg(const TCHAR* file_name, const TCHAR* block_def_name);
    static Acad::ErrorStatus InsertDwgAsBlockDef(const TCHAR* file_name, const TCHAR* block_name, bool overwrite, AcDbObjectId& id, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
    static AcDbObjectId InsertDwgAsBlockDef(const TCHAR* file_name, const TCHAR* block_name, bool overwrite, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
};
