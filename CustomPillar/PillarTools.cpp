#include "stdafx.h"
#include "PillarTools.h"
#include "SelectEntitys.h"

bool PillarTools::detectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter)
{
	AcDbObjectIdArray all_pillar_ids;
	SelectEntitys::GetAllEntitysByType(CPolaCustomPillar::desc(), all_pillar_ids);
	AcDbExtents extents;
	for (const auto& pillar : all_pillar_ids)
	{
		AcDbEntity* entity = nullptr;
		Acad::ErrorStatus error_status = acdbOpenObject(entity, pillar, OpenMode::kForRead);
		if (error_status != Acad::eOk)
			return false;
		CPolaCustomPillar* pillar = CPolaCustomPillar::cast(entity);
		pillar->getGeomExtents(extents);
		if (BasicTools::IsPointInRectangle(point, extents))
		{
			pillar_id = entity->objectId();
			double temp_d, temp_h;
			pillar->getDiameter(temp_d, temp_h);
			pillar_diameter = temp_d;
			pillar->close();
			return true;
		}
		entity->close();
	}
	pillar_id = AcDbObjectId::kNull;
	pillar_diameter = 0;
	return false;
}
