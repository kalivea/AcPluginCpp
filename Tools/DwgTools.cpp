#include "stdafx.h"
#include "DwgTools.h"

bool DwgTools::GetAcadPath(CString& acad_path)
{
	DWORD dword_return = ::GetModuleFileNameW(acedGetAcadWinApp()->m_hInstance, acad_path.GetBuffer(_MAX_PATH), _MAX_PATH);
	acad_path.ReleaseBuffer();

	if (dword_return == 0)
		return false;
	else
		return true;
}

bool DwgTools::SaveDwgFiles(AcDbDatabase* database, CString& save_path, CString& dwg_name)
{
	CString file_path;
	file_path = save_path + dwg_name;

	database->saveAs(file_path);
	delete database;
}

AcDbExtents DwgTools::GetModelExtent(AcDbDatabase* database)
{
	AcDbBlockTable* block_table = nullptr;
	database->getBlockTable(block_table);

	AcDbBlockTableRecord* block_table_record = nullptr;
	block_table->getAt(ACDB_MODEL_SPACE, block_table_record);

}

void DwgTools::ZoomDwgExtent(AcDbDatabase* database)
{
	assert(database);

	AcDbExtents extents;
}

AcDbObjectId DwgTools::CopyBlockDefFromOtherDwg(const TCHAR* file_name, const TCHAR* block_def_name)
{
	AcDbObjectId block_ref_id = AcDbObjectId::kNull;

	AcDbDatabase* source_dwg = new AcDbDatabase(false);
	Acad::ErrorStatus error_status = source_dwg->readDwgFile(file_name, AcDbDatabase::OpenMode::kForReadAndAllShare);

	if (error_status != Acad::eOk)
	{
		delete source_dwg;
		source_dwg = nullptr;
		return block_ref_id;
	}
	AcDbBlockTable* block_table = nullptr;

	error_status = source_dwg->getBlockTable(block_table, AcDb::kForRead);
	assert(es == Acad::eOk);
}
