#include "stdafx.h"
#include "EditBlock.h"

bool EditBlock::IsBlockExist(const TCHAR* block_name, AcDbDatabase* database)
{
	AcDbBlockTable* block_table = nullptr;
	database->getBlockTable(block_table, OpenMode::kForRead);
	if (block_table->has(block_name))
	{
		block_table->close();
		return true;
	}
	else
	{
		block_table->close();
		return false;
	}
}

AcDbObjectId EditBlock::GetBlockId(const TCHAR* block_name, AcDbDatabase* database)
{
	AcDbBlockTable* block_table = nullptr;
	AcDbObjectId block_object_id = AcDbObjectId::kNull;
	database->getBlockTable(block_table, OpenMode::kForRead);
	if (IsBlockExist(block_name))
	{
		block_table->getAt(block_name, block_object_id);
		block_table->close();
	}
	else
	{
		block_table->close();
	}
	return block_object_id;
}

bool EditBlock::SetBlockInsertPointToCenter(AcDbObjectId blockDefId)
{
	AcDbBlockTableRecord* block_def = nullptr;
	if (acdbOpenObject(block_def, blockDefId, AcDb::kForWrite) != Acad::eOk)
		return false;

	AcDbExtents block_extents;
	bool hasExtents = false;

	AcDbBlockTableRecordIterator* block_table_record_iterator = nullptr;
	block_def->newIterator(block_table_record_iterator);

	for (block_table_record_iterator->start(); !block_table_record_iterator->done(); block_table_record_iterator->step())
	{
		AcDbEntity* entity = nullptr;
		if (block_table_record_iterator->getEntity(entity, AcDb::kForRead) == Acad::eOk)
		{
			AcDbExtents entity_extents;
			if (entity->getGeomExtents(entity_extents) == Acad::eOk)
			{
				if (!hasExtents)
				{
					block_extents = entity_extents;
					hasExtents = true;
				}
				else
				{
					block_extents.addExt(entity_extents);
				}
			}
			entity->close();
		}
	}

	if (!hasExtents)
	{
		delete block_table_record_iterator;
		block_def->close();
		return false;
	}

	AcGePoint3d center_point = BasicTools::GetMidPoint(block_extents);

	delete block_table_record_iterator;
	block_def->newIterator(block_table_record_iterator);

	for (block_table_record_iterator->start(); !block_table_record_iterator->done(); block_table_record_iterator->step())
	{
		AcDbEntity* entity = nullptr;
		if (block_table_record_iterator->getEntity(entity, AcDb::kForWrite) == Acad::eOk)
		{
			AcGeMatrix3d xform_matrix;
			xform_matrix.setTranslation(-center_point.asVector());
			entity->transformBy(xform_matrix);
			entity->close();
		}
	}

	delete block_table_record_iterator;
	block_def->close();
	return true;
}

AcDbObjectId EditBlock::InsertBlockRef(AcDbObjectId block_def_id, const AcGePoint3d& insert_point, double scale, double rotation)
{
	AcDbBlockReference* block_ref = new AcDbBlockReference(insert_point, block_def_id);
	block_ref->setRotation(rotation);
	SetScale(block_ref, scale);
	return AddToModelSpace::AddEntityToModelSpace(block_ref);
}

void EditBlock::SetScale(AcDbBlockReference* block_ref, double scale)
{
	if (block_ref == nullptr)
		throw;
	AcGeScale3d block_scale(scale, scale, scale);
	block_ref->setScaleFactors(block_scale);
}
