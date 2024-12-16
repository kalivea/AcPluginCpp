#include "StdAfx.h"
#include "EditEntity.h"

void EditEntity::SetColor(const AcDbObjectId& entity_id, const unsigned char& color_index)
{
	AcDbEntity* entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}

	entity->setColorIndex(color_index);
	entity->close();
}

void EditEntity::SetLayer(const AcDbObjectId& entity_id, TCHAR* layer_name)
{
	AcDbEntity* entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	if (StyleTools::IsLayerExist(layer_name))
	{
		entity->setLayer(layer_name);
		entity->close();
	}
	else
	{
		AcDbDatabase* database = nullptr;
		database = acdbHostApplicationServices()->workingDatabase();
		AcDbLayerTable* layer_table = nullptr;
		acdbOpenObject(layer_table, database->layerTableId(), OpenMode::kForWrite);

		AcDbLayerTableRecord* layer_table_record = nullptr;
		layer_table_record = new AcDbLayerTableRecord();
		layer_table_record->setName(layer_name);							//only add layer name,need more param.
		layer_table->add(layer_table_record);
		layer_table_record->close();
		layer_table->close();
		entity->close();

		SetLayer(entity_id, layer_name);
	}
}

void EditEntity::MoveEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point)
{
	AcDbEntity* entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	AcGeVector3d vector = BasicTools::GetVectorBetweenTwoPoint(base_point, target_point);
	AcGeMatrix3d move_transform_matrix;
	move_transform_matrix.setToTranslation(vector);
	entity->transformBy(move_transform_matrix);
	entity->close();
}

AcDbObjectId EditEntity::CopyEntity(const AcDbObjectId& base_entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point)
{
	AcDbEntity* base_entity = nullptr;
	AcDbEntity* copy_entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(base_entity, base_entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	AcGeVector3d vector = BasicTools::GetVectorBetweenTwoPoint(base_point, target_point);
	AcGeMatrix3d copy_transform_matrix;
	copy_transform_matrix.setToTranslation(vector);
	base_entity->getTransformedCopy(vector, copy_entity);
	base_entity->close();
	return AddToModelSpace::AddEntityToModelSpace(copy_entity);
}

void EditEntity::RotateEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& angle, char* angle_type)
{
	AcDbEntity* entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	double temp_angle;
	if (angle_type == "DEG")
		temp_angle = BasicTools::ConvertAngle(angle, "RAD");
	else if (angle_type == "RAD")
		temp_angle = angle;
	else
		throw"Wrong angle type. Only \"DEG\",\"RAD\" can be use!~";
	AcGeMatrix3d rotate_transform_matrix;
	rotate_transform_matrix.setToRotation(temp_angle, AcGeVector3d::kZAxis, base_point);
	entity->transformBy(rotate_transform_matrix);
	entity->close();
}

void EditEntity::DeleteEntity(const AcDbObjectId& entity_id)
{
	AcDbEntity* entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	entity->erase();
	entity->close();
}

void EditEntity::ScaleEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& scale)
{
	AcDbEntity* entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	AcGeMatrix3d scale_transform_matrix;
	scale_transform_matrix.setToScaling(scale, base_point);
	entity->transformBy(scale_transform_matrix);
	entity->close();
}

AcDbObjectId EditEntity::MirrorEntity(const AcDbObjectId& entity_id, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, bool need_delete_original_entity)
{
	AcDbEntity* mirror_entity = nullptr;
	AcDbEntity* base_entity = nullptr;
	ErrorStatus err_status = acdbOpenObject(base_entity, entity_id, OpenMode::kForWrite);
	if (err_status != Acad::eOk)
	{
		throw;						//TODO: improve handling logic.
	}
	AcGeLine3d mirror_axis(mirror_point1, mirror_point2);
	AcGeMatrix3d mirror_transform_matrix;
	mirror_transform_matrix.setToMirroring(mirror_axis);
	base_entity->getTransformedCopy(mirror_transform_matrix, mirror_entity);
	if (need_delete_original_entity)
	{
		base_entity->erase();
	}
	base_entity->close();
	return AddToModelSpace::AddEntityToModelSpace(mirror_entity);
}