#include "stdafx.h"
#include "DwgTools.h"
#include "BasicTools.h"

AcDbObjectId DwgTools::CopyBlockDefFromOtherDwg(const TCHAR* file_name, const TCHAR* block_def_name)
{
	AcDbObjectId block_ref_id = AcDbObjectId::kNull;

	AcDbDatabase* source_dwg = new AcDbDatabase(false);
	Acad::ErrorStatus error_status = source_dwg->readDwgFile(file_name, AcDbDatabase::OpenMode::kForReadAndAllShare);

	if (error_status != Acad::eOk)
	{
		delete source_dwg;
		source_dwg = nullptr;
		acutPrintf(_T("Read dwg file Error,file name: %s"), file_name);
		return block_ref_id;
	}
	AcDbBlockTable* block_table = nullptr;

	error_status = source_dwg->getBlockTable(block_table, AcDb::kForRead);
	if (error_status != Acad::eOk)
	{
		delete source_dwg;
		source_dwg = nullptr;
		acutPrintf(_T("Failed to get block table, error: %d"), error_status);
		return block_ref_id;
	}

	if (block_table->has(block_def_name))
	{
		AcDbObjectId target_block_def_id;
		error_status = block_table->getAt(block_def_name, target_block_def_id);
		if (error_status != Acad::eOk)
		{
			block_table->close();
			delete source_dwg;
			source_dwg = nullptr;
			acutPrintf(_T("Failed to get block definition, error: %d"), error_status);
			return block_ref_id;
		}

		AcDbDatabase* temp_database = nullptr;
		error_status = source_dwg->wblock(temp_database, target_block_def_id);
		if (error_status != Acad::eOk)
		{
			block_table->close();
			delete source_dwg;
			source_dwg = nullptr;
			acutPrintf(_T("Failed to wblock, error: %d"), error_status);
			return block_ref_id;
		}

		AcDbDatabase* current_database = acdbHostApplicationServices()->workingDatabase();
		error_status = current_database->insert(block_ref_id, block_def_name, temp_database);
		if (error_status != Acad::eOk)
		{
			delete temp_database;
			temp_database = nullptr;
			block_table->close();
			delete source_dwg;
			source_dwg = nullptr;
			acutPrintf(_T("Failed to insert block, error: %d"), error_status);
			return block_ref_id;
		}

		delete temp_database;
		temp_database = nullptr;
	}
	else
	{
		acutPrintf(_T("No block id in dwg files"));
	}
	block_table->close();
	delete source_dwg;
	source_dwg = nullptr;
	return block_ref_id;
}

AcDbObjectId DwgTools::InsertDwgBlockDef(TCHAR* file_name, TCHAR* block_name, bool overwrite, AcDbDatabase* database)
{
	try
	{
		AcDbObjectId block_def_id = BasicTools::GetBlockId(block_name, database);

		if (!block_def_id.isNull() && overwrite)
		{
			AcDbBlockTableRecord* block_table_record;
			if (acdbOpenObject(block_table_record, block_def_id, AcDb::kForWrite) == Acad::eOk)
			{
				block_table_record->erase();
				block_table_record->close();
				block_def_id = AcDbObjectId::kNull;
			}
		}

		if (block_def_id.isNull())
		{
			if (_taccess(file_name, 0) == -1)
			{
				acutPrintf(_T("Can't find DWG file '%s'"), file_name);
				return AcDbObjectId::kNull;
			}

			std::unique_ptr<AcDbDatabase> block_database(new AcDbDatabase(false));
			Acad::ErrorStatus error_status = block_database->readDwgFile(file_name, AcDbDatabase::OpenMode::kForReadAndAllShare);

			if (error_status != Acad::eOk)
			{
				acutPrintf(_T("Read DWG file '%s' failed! Error Code:%d"),
					file_name, static_cast<int>(error_status));
				return AcDbObjectId::kNull;
			}

			error_status = database->insert(block_def_id, block_name, block_database.get());
			if (error_status != Acad::eOk)
			{
				acutPrintf(_T("Insert block '%s' from file '%s' failed! Error Code:%d"),
					block_name, file_name, static_cast<int>(error_status));
				return AcDbObjectId::kNull;
			}


			if (!BasicTools::SetBlockInsertPointToCenter(block_def_id))
			{
				acutPrintf(_T("Failed to set block insert point to center for '%s'"), block_name);
			}
		}
		return block_def_id;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("Exception during InsertDwgBlockDef: %s"), e.what());
		return AcDbObjectId::kNull;
	}
	catch (...)
	{
		acutPrintf(_T("Unknown exception during InsertDwgBlockDef"));
		return AcDbObjectId::kNull;
	}
}
