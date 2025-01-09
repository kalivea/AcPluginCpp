#include "StdAfx.h"
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

AcDbObjectIdArray AddToModelSpace::AddEntityToModelSpace(std::vector<AcDbEntity*> entity_pointer_vector)
{
	AcDbObjectIdArray entity_id_array;
	for (int i = 0; i < entity_pointer_vector.size(); i++)
	{
		if (entity_pointer_vector.at(i) == nullptr)				//check pointer
			throw;									//TODO:Improve exception handling logic
		else
			entity_id_array.append(AddToModelSpace::AddEntityToModelSpace(entity_pointer_vector.at(i)));
	}
	return entity_id_array;
}

