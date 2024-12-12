#pragma once
#include "def.h"
#define POLA_DLL
/*
* Update Time: 2024-12-04
* This is a collection of tools that can edit CAD entity(s).
* It can scale, move, copy, etc CAD entiy(s).
* 
*/
class POLADLL EditEntity
{
public:
	// property edit
	static void SetColor(const AcDbObjectId& entity_id, const unsigned char& color_index);
	static void SetLayer(const AcDbObjectId& entity_id,TCHAR* layer_name);		//set layer name only.

	// entity edit
	static void MoveEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static AcDbObjectId CopyEntity(const AcDbObjectId& base_entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static void RotateEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& angle, char* angle_type);
	static void DeleteEntity(const AcDbObjectId& entity_id);
	static void ScaleEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& scale);
	static AcDbObjectId MirrorEntity(const AcDbObjectId& entity_id, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, bool need_delete_original_entity);


};

