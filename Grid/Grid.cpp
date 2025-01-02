#include "StdAfx.h"
#include "Grid.h"

AcDbObjectId Grid::CreateVerticalAxis()
{
	AcDbBlockTable* block_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(block_table, OpenMode::kForWrite);
	AcDbBlockTableRecord* block_table_record = new AcDbBlockTableRecord();

	block_table_record->setName(_T("grid_v"));

	AcDbObjectId grid_vertical_id = AcDbObjectId::kNull;
	block_table->add(grid_vertical_id, block_table_record);

	AcDbCircle* grid_vertical_circle = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, circle_radius_);
	AcDbLine* grid_vertical_line = new AcDbLine(
		AcGePoint3d(AcGePoint3d::kOrigin.x, AcGePoint3d::kOrigin.y + grid_vertical_circle->radius(), 0),
		AcGePoint3d(AcGePoint3d::kOrigin.x, AcGePoint3d::kOrigin.y + grid_vertical_circle->radius() + line_length_, 0));

	AcDbAttributeDefinition* grid_vertical_attribute = new AcDbAttributeDefinition(AcGePoint3d::kOrigin, _T("index_num"), _T("ID"), _T("Input axis id"), StyleTools::GetTextStyleId(_T("grid_text")));
	grid_vertical_attribute->setHeight(750);
	grid_vertical_attribute->setHorizontalMode(AcDb::kTextMid);

	AcDbObjectId circel_id, line_id, attribute_id;
	block_table_record->appendAcDbEntity(circel_id, grid_vertical_circle);
	block_table_record->appendAcDbEntity(line_id, grid_vertical_line);
	block_table_record->appendAcDbEntity(attribute_id, grid_vertical_attribute);

	grid_vertical_circle->close();
	grid_vertical_line->close();
	grid_vertical_attribute->close();
	block_table_record->close();
	block_table->close();

	return grid_vertical_id;
}

AcDbObjectId Grid::CreateHorizonAxis()
{
	AcDbBlockTable* block_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(block_table, OpenMode::kForWrite);
	AcDbBlockTableRecord* block_table_record = new AcDbBlockTableRecord();

	block_table_record->setName(_T("grid_h"));

	AcDbObjectId gride_vertical_id = AcDbObjectId::kNull;
	block_table->add(gride_vertical_id, block_table_record);

	AcDbCircle* grid_vertical_circle = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, circle_radius_);
	AcDbLine* gride_vertical_line = new AcDbLine(
		AcGePoint3d(AcGePoint3d::kOrigin.x + grid_vertical_circle->radius(), AcGePoint3d::kOrigin.y, 0),
		AcGePoint3d(AcGePoint3d::kOrigin.x + grid_vertical_circle->radius() + line_length_, AcGePoint3d::kOrigin.y, 0));

	AcDbAttributeDefinition* gride_vertical_attribute = new AcDbAttributeDefinition(AcGePoint3d::kOrigin, _T("index_num"), _T("ID"), _T("Input axis id"), StyleTools::GetTextStyleId(_T("grid_text")));
	gride_vertical_attribute->setHeight(750);
	gride_vertical_attribute->setHorizontalMode(AcDb::kTextMid);

	AcDbObjectId circel_id, line_id, attribute_id;
	block_table_record->appendAcDbEntity(circel_id, grid_vertical_circle);
	block_table_record->appendAcDbEntity(line_id, gride_vertical_line);
	block_table_record->appendAcDbEntity(attribute_id, gride_vertical_attribute);

	grid_vertical_circle->close();
	gride_vertical_line->close();
	gride_vertical_attribute->close();
	block_table_record->close();
	block_table->close();

	return gride_vertical_id;
}

void Grid::AddAttribute(AcDbBlockReference* block_reference, AcDbAttributeDefinition* att_definition)
{
	AcDbAttribute* attribute = new AcDbAttribute();
	attribute->setPropertiesFrom(att_definition);
	attribute->setHeight(att_definition->height());
	attribute->setHorizontalMode(att_definition->horizontalMode());
	attribute->setTextStyle(att_definition->textStyle());
	attribute->setPosition(att_definition->position());
	attribute->setAttributeFromBlock(block_reference->blockTransform());

	TCHAR* att_string = att_definition->tag();
	attribute->setTag(att_string);
	acutDelString(att_string);
	attribute->setFieldLength(att_definition->fieldLength());
	attribute->setTextString(att_definition->textString());

	block_reference->appendAttribute(attribute);
	attribute->close();
}

void Grid::SetAttribute(AcDbBlockReference* block_reference, TCHAR* tag, TCHAR* att_string)
{
	AcDbBlockTableRecord* block_record = nullptr;
	acdbOpenObject(block_record, block_reference->blockTableRecord());
	if (block_record->hasAttributeDefinitions())
	{
		AcDbBlockTableRecordIterator* block_table_record_iterator = nullptr;
		block_record->newIterator(block_table_record_iterator);
		AcDbEntity* entity = nullptr;
		for (block_table_record_iterator->start(); !block_table_record_iterator->done(); block_table_record_iterator->step())
		{
			block_table_record_iterator->getEntity(entity, OpenMode::kForRead);
			AcDbAttributeDefinition* att_definiton = AcDbAttributeDefinition::cast(entity);

			if (att_definiton != nullptr)
			{
				TCHAR* check_tag = att_definiton->tag();
				CString tag_string = check_tag;
				acutDelString(check_tag);
				if (tag_string.CompareNoCase(tag) == 0)
				{
					bool IsfoundTag = false;
					AcDbObjectIterator* att_iterator = block_reference->attributeIterator();
					for (att_iterator->start(); !att_iterator->done(); att_iterator->step())
					{
						AcDbAttribute* attribute = nullptr;
						Acad::ErrorStatus err_status = acdbOpenObject(attribute, att_iterator->objectId(), OpenMode::kForWrite);
						if (err_status == Acad::eOk)
						{
							check_tag = attribute->tag();
							tag_string = check_tag;
							acutDelString(check_tag);
							if (tag_string.CompareNoCase(tag) == 0)
							{
								attribute->setTextString(att_string);
								IsfoundTag = true;
							}
							attribute->close();
						}
					}
					delete att_iterator;
				}
			}
			entity->close();
		}
		delete block_table_record_iterator;
	}
	block_record->close();
}

AcDbObjectId Grid::InsertVerticalAxis(AcGePoint3d insert_point)
{
	AcDbBlockReference* block_reference = new AcDbBlockReference(insert_point, BasicTools::GetBlockId(_T("grid_v")));
	AcDbBlockTableRecord* block_table_record = nullptr;
	acdbOpenObject(block_table_record, block_reference->blockTableRecord());
	if (block_table_record->hasAttributeDefinitions())
	{
		AcDbBlockTableRecordIterator* block_table_record_iterator = nullptr;
		block_table_record->newIterator(block_table_record_iterator);
		for (block_table_record_iterator->start(); !block_table_record_iterator->done(); block_table_record_iterator->step())
		{
			AcDbEntity* entity = nullptr;
			if (block_table_record_iterator->getEntity(entity) == Acad::eOk)
			{
				AcDbAttributeDefinition* attribute_definition = AcDbAttributeDefinition::cast(entity);
				if (attribute_definition != nullptr)
				{
					AddAttribute(block_reference, attribute_definition);
				}
				entity->close();
			}
		}
		delete block_table_record_iterator;
	}
	block_table_record->close();
	return AddToModelSpace::AddEntityToModelSpace(block_reference);
}

AcDbObjectId Grid::InsertHorizonAxis(AcGePoint3d insert_point)
{
	AcDbBlockReference* block_reference = new AcDbBlockReference(insert_point, BasicTools::GetBlockId(_T("grid_h")));
	AcDbBlockTableRecord* block_table_record = nullptr;
	acdbOpenObject(block_table_record, block_reference->blockTableRecord());
	if (block_table_record->hasAttributeDefinitions())
	{
		AcDbBlockTableRecordIterator* block_table_record_iterator = nullptr;
		block_table_record->newIterator(block_table_record_iterator);
		for (block_table_record_iterator->start(); !block_table_record_iterator->done(); block_table_record_iterator->step())
		{
			AcDbEntity* entity = nullptr;
			if (block_table_record_iterator->getEntity(entity) == Acad::eOk)
			{
				AcDbAttributeDefinition* attribute_definition = AcDbAttributeDefinition::cast(entity);
				if (attribute_definition != nullptr)
				{
					AddAttribute(block_reference, attribute_definition);
				}
				entity->close();
			}
		}
		delete block_table_record_iterator;
	}
	block_table_record->close();
	return AddToModelSpace::AddEntityToModelSpace(block_reference);
}

void Grid::DrawVerticalAxis(AcGePoint3dArray insert_point, TCHAR* insert_att[])
{
	for (int i = 0; i < insert_point.length(); i++)
	{
		AcDbBlockReference* temp_reference = nullptr;
		AcDbObjectId grid_id = Grid::InsertVerticalAxis(insert_point[i]);
		acdbOpenObject(temp_reference, grid_id, OpenMode::kForWrite);
		Grid::SetAttribute(temp_reference, _T("ID"), insert_att[i]);
		temp_reference->close();
	}
}

void Grid::DrawHorizonAxis(AcGePoint3dArray insert_point, TCHAR* insert_att[])
{
	for (int i = 0; i < insert_point.length(); i++)
	{
		AcDbBlockReference* temp_reference = nullptr;
		AcDbObjectId grid_id = Grid::InsertHorizonAxis(insert_point[i]);
		acdbOpenObject(temp_reference, grid_id, OpenMode::kForWrite);
		Grid::SetAttribute(temp_reference, _T("ID"), insert_att[i]);
		temp_reference->close();
	}
}
