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

bool PillarTools::detectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter)
{
	AcDbObjectId pillar;
	double diameter;
	double height = 0;
	if (detectPillar(point, pillar, diameter))
	{
		AcDbEntity* entity = nullptr;
		Acad::ErrorStatus error_status = acdbOpenObject(entity, pillar, OpenMode::kForRead);
		if (error_status != Acad::eOk)
			return false;
		CPolaCustomPillar* pillar_entity = CPolaCustomPillar::cast(entity);
		if (pillar_entity->getPillarType() == 0)
		{
			pillar_entity->close();
			pillar_id = pillar_entity->objectId();
			pillar_diameter = diameter;
			return true;
		}
		pillar_entity->close();
	}
	return false;
}

bool PillarTools::GetAllPillar(AcDbObjectIdArray& all_pillar_ids)
{
	return SelectEntitys::GetAllEntitysByType(CPolaCustomPillar::desc(), all_pillar_ids);
}
