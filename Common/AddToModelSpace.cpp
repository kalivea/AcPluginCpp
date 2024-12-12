#include "stdafx.h"
#include "AddToModelSpace.h"

AcDbObjectId AddToModelSpace::AddEntityToModelSpace(AcDbEntity* entity_pointer)
{
	if (entity_pointer == nullptr)				//check pointer
		throw;									//TODO:Improve exception handling logic
	AcDbBlockTable* block_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(block_table, OpenMode::kForRead);

	AcDbBlockTableRecord* block_table_record = nullptr;
	block_table->getAt(ACDB_MODEL_SPACE, block_table_record, OpenMode::kForWrite);

	AcDbObjectId entity_id = AcDbObjectId::kNull;
	ErrorStatus err_status = block_table_record->appendAcDbEntity(entity_id, entity_pointer);

	if (err_status != Acad::eOk)				//if status is error handle the exception
	{
		block_table_record->close();
		delete entity_pointer;
		entity_pointer = nullptr;
		return AcDbObjectId::kNull;
		throw;									//TODO:Improve exception handling logic
	}

	block_table_record->close();				//close all
	block_table->close();
	entity_pointer->close();

	return entity_id;							//return entity id
}
