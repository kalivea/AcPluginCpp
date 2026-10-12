#pragma once
#include "dbmain.h"
#include "dbapserv.h"
#include <tchar.h>
#include "CommonDef.h"

class POLADLL_COMMON_API EditBlock
{
public:
    static Acad::ErrorStatus IsBlockExist(const TCHAR* block_name, bool& exists, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
    static bool IsBlockExist(const TCHAR* block_name, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
    static Acad::ErrorStatus GetBlockId(const TCHAR* block_name, AcDbObjectId& id, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
    static AcDbObjectId GetBlockId(const TCHAR* block_name, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
    static Acad::ErrorStatus CenterBlockInsertPoint(AcDbObjectId block_def_id);
    static bool SetBlockInsertPointToCenter(AcDbObjectId block_def_id);
    static Acad::ErrorStatus InsertBlockRef(AcDbObjectId block_def_id, const AcGePoint3d& insert_point, double scale, double rotation, AcDbObjectId& id);
    static AcDbObjectId InsertBlockRef(AcDbObjectId block_def_id, const AcGePoint3d& insert_point, double scale, double rotation);
    // Borrowed reference; this operation never closes or deletes block_ref.
    static Acad::ErrorStatus SetScale(AcDbBlockReference* block_ref, double scale);
};
