#include "StdAfx.h"
#include "SelectEntitys.h"

bool SelectEntitys::PickEntitys(const TCHAR* prompt, resbuf* result_buff, AcDbObjectIdArray& entity_id_array)
{
	entity_id_array.removeAll();
	entity_id_array.setLogicalLength(0);

	bool result = false;
	ads_name selection_set_name;
	acutPrintf(prompt);

	if (acedSSGet(nullptr, nullptr, nullptr, result_buff, selection_set_name) == RTNORM)
	{
		int length = 0;
		acedSSLength(selection_set_name, &length);
		for (int i = 0; i < length; i++)
		{
			ads_name entity_name;
			acedSSName(selection_set_name, i, entity_name);
			AcDbObjectId entity_id = AcDbObjectId::kNull;
			acdbGetObjectId(entity_id, entity_name);
			entity_id_array.append(entity_id);
		}
		result = true;
	}
	else
	{
		result = false;
	}
	acedSSFree(selection_set_name);
	return result;
}

bool SelectEntitys::PickEntitys(const TCHAR* prompt, const std::vector<AcRxClass*>& class_describe, AcDbObjectIdArray& entity_id_array)
{
	entity_id_array.removeAll();
	entity_id_array.setLogicalLength(0);
	ads_name selection_set_name;
	int result = 0;
	acutPrintf(prompt);

	if (acedSSGet(nullptr, nullptr, nullptr, nullptr, selection_set_name) != RTNORM)
	{
		return false;
	}

	int length = 0;
	acedSSLength(selection_set_name, &length);

	Acad::ErrorStatus error_status;
	AcDbEntity* entity = nullptr;
	for (int i = 0; i < length; i++)
	{
		ads_name entity_name;
		acedSSName(selection_set_name, i, entity_name);
		AcDbObjectId current_entity_id = AcDbObjectId::kNull;
		error_status = acdbGetObjectId(current_entity_id, entity_name);
		if (error_status != Acad::eOk)
		{
			continue;
		}
		error_status = acdbOpenAcDbEntity(entity, current_entity_id, AcDb::kForRead);
		if (error_status != Acad::eOk)
		{
			continue;
		}
		for (int j = 0; j < class_describe.size(); j++)
		{
			if (entity->isKindOf(class_describe.at(j)))
			{
				entity_id_array.append(entity->objectId());
				break;
			}
		}
		entity->close();
	}
	acedSSFree(selection_set_name);
	return entity_id_array.isEmpty() ? false : true;
}

bool SelectEntitys::PickEntitys(const TCHAR* prompt, AcRxClass* class_describe, AcDbObjectIdArray& entity_id_array)
{
	std::vector<AcRxClass*> class_describe_vector;
	class_describe_vector.push_back(class_describe);
	return PickEntitys(prompt, class_describe_vector, entity_id_array);
}

bool SelectEntitys::PickLinesOnLayer(const TCHAR* layer_name, AcDbObjectIdArray& entity_id_array)
{
	struct resbuf* result_buff = acutBuildList(RTDXF0, _T("LINE"), 8, layer_name, RTNONE);
	bool result = PickEntitys(_T("Select lines on layer: "), result_buff, entity_id_array);
	acutRelRb(result_buff);
	return result;
}

bool SelectEntitys::PickEntitysInRectangel(const TCHAR* prompt, const AcGePoint2d& point1, const AcGePoint2d& point2, AcDbObjectIdArray& entity_id_array)
{
	entity_id_array.removeAll();
	entity_id_array.setLogicalLength(0);

	if (BasicTools::CanDrawRect(BasicTools::Point2dToPoint3d(point1), BasicTools::Point2dToPoint3d(point2)))
	{
		AcDbObjectIdArray all_entity_ids = BasicTools::GetAllEntityIdsInDatabase();
		for (int i = 0; i < all_entity_ids.length(); i++)
		{
			AcDbEntity* entity = nullptr;
			if (acdbOpenObject(entity, all_entity_ids.at(i), AcDb::kForRead) == Acad::eOk)
			{
				AcDbExtents extents;
				entity->getGeomExtents(extents);
				AcGePoint3d extents_min_point = extents.minPoint();
				AcGePoint3d extents_max_point = extents.maxPoint();
				if (BasicTools::IsIntersectRectangle(BasicTools::Point2dToPoint3d(point1), BasicTools::Point2dToPoint3d(point2), extents_min_point, extents_max_point))
				{
					entity_id_array.append(entity->objectId());
				}
				entity->close();
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
