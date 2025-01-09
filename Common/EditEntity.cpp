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

void EditEntity::SetColor(const AcDbObjectIdArray& entity_ids, const unsigned char& color_index)
{
	for (int i = 0; i < entity_ids.length(); i++)
	{
		SetColor(entity_ids.at(i), color_index);
	}
}

void EditEntity::SetColor(const AcDbObjectIdArray& entity_id, const unsigned char color_index[])
{
	for (int i = 0; i < entity_id.length(); i++)
	{
		SetColor(entity_id.at(i), color_index[i]);
	}
}

void EditEntity::SetLayer(const AcDbObjectId& entity_id, const TCHAR* layer_name)
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

void EditEntity::SetLayer(const AcDbObjectIdArray& entity_ids, const TCHAR* layer_name)
{
	for (int i = 0; i < entity_ids.length(); i++)
	{
		SetLayer(entity_ids.at(i), layer_name);
	}
}

void EditEntity::SetLayer(const AcDbObjectIdArray& entity_ids, const TCHAR* layer_name[])
{
	for (int i = 0; i < entity_ids.length(); i++)
	{
		SetLayer(entity_ids.at(i), layer_name[i]);
	}
}

void EditEntity::SetLinetype(const AcDbObjectId& entity_id, const TCHAR* linetype_name, const double& line_type_scale)
{
	if (StyleTools::IsLineTypeExist(linetype_name))
	{
		AcDbEntity* entity = nullptr;
		ErrorStatus err_status = acdbOpenObject(entity, entity_id, OpenMode::kForWrite);
		if (err_status != Acad::eOk)
		{
			throw;						//TODO: improve handling logic.
		}
		entity->setLinetype(linetype_name);
		entity->setLinetypeScale(line_type_scale);
		entity->close();
	}
	else
	{
		StyleTools::LoadLineType(linetype_name, _T("acad.lin"));
		SetLinetype(entity_id, linetype_name, line_type_scale);
	}
}

void EditEntity::SetLinetype(const AcDbObjectIdArray& entity_ids, const TCHAR* linetype_name, const double& line_type_scale)
{
	for (int i = 0; i < entity_ids.length(); i++)
	{
		SetLinetype(entity_ids.at(i), linetype_name, line_type_scale);
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

void EditEntity::MoveEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const AcGePoint3dArray& target_point)
{
	if (entity_ids.length() != base_point.length() || entity_ids.length() != target_point.length())
	{
		throw;						//TODO: improve handling logic.
	}
	for (int i = 0; i < entity_ids.length(); i++)
	{
		MoveEntity(entity_ids.at(i), base_point.at(i), target_point.at(i));
	}
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

AcDbObjectIdArray EditEntity::CopyEntity(const AcDbObjectIdArray& base_entity_ids, const AcGePoint3dArray& base_point, const AcGePoint3dArray& target_point)
{
	AcDbObjectIdArray copy_entity_ids;
	if (base_entity_ids.length() != base_point.length() || base_entity_ids.length() != target_point.length())
	{
		throw;						//TODO: improve handling logic.
	}
	for (int i = 0; i < base_entity_ids.length(); i++)
	{
		copy_entity_ids.append(CopyEntity(base_entity_ids.at(i), base_point.at(i), target_point.at(i)));
	}
	return copy_entity_ids;
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
		temp_angle = BasicTools::ConvertAngle(angle, 1);
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

void EditEntity::DeleteEntity(const AcDbObjectIdArray& entity_ids)
{
	for (int i = 0; i < entity_ids.length(); i++)
	{
		DeleteEntity(entity_ids.at(i));
	}
}

void EditEntity::ScaleEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& scale)
{
	if (scale <= 0)
	{
		throw;						//TODO: improve handling logic.
	}

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

void EditEntity::ScaleEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const double& scale)
{
	if (entity_ids.length() != base_point.length())
	{
		throw;						//TODO: improve handling logic.
	}
	for (int i = 0; i < entity_ids.length(); i++)
	{
		ScaleEntity(entity_ids.at(i), base_point.at(i), scale);
	}
}

AcDbObjectId EditEntity::MirrorEntity(const AcDbObjectId& entity_id, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, const bool need_delete_original_entity)
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

AcDbObjectIdArray EditEntity::MirrorEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, const bool need_delete_original_entity)
{
	AcDbObjectIdArray mirror_entity_ids;
	for (int i = 0; i < entity_ids.length(); i++)
	{
		mirror_entity_ids.append(MirrorEntity(entity_ids.at(i), mirror_point1, mirror_point2, need_delete_original_entity));
	}
	return mirror_entity_ids;
}