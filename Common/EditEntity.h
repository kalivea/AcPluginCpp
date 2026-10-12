#pragma once
#include "dbmain.h"
#include "gept3dar.h"
#include <tchar.h>
#include "CommonDef.h"
/*
* Update Time: 2024-12-04
* This is a collection of tools that can edit CAD entity(s).
* It can scale, move, copy, etc CAD entiy(s).
*
*/
class POLADLL_COMMON_API EditEntity
{
public:
	static Acad::ErrorStatus CopyEntity(const AcDbObjectId& base_entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point, AcDbObjectId& copy_id);
	static Acad::ErrorStatus MirrorEntity(const AcDbObjectId& entity_id, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, bool need_delete_original_entity, AcDbObjectId& mirror_id);
	// property edit
	static Acad::ErrorStatus SetColor(const AcDbObjectId& entity_id, const unsigned char& color_index);
	static Acad::ErrorStatus SetColor(const AcDbObjectIdArray& entity_ids, const unsigned char& color_index);
	static Acad::ErrorStatus SetColor(const AcDbObjectIdArray& entity_id, const unsigned char color_index[]);

	static Acad::ErrorStatus SetLayer(const AcDbObjectId& entity_id, const TCHAR* layer_name);
	static Acad::ErrorStatus SetLayer(const AcDbObjectIdArray& entity_ids, const TCHAR* layer_name);
	static Acad::ErrorStatus SetLayer(const AcDbObjectIdArray& entity_ids, const TCHAR* layer_name[]);

	static Acad::ErrorStatus SetLinetype(const AcDbObjectId& entity_id, const TCHAR* linetype_name, const double& line_type_scale);
	static Acad::ErrorStatus SetLinetype(const AcDbObjectIdArray& entity_ids, const TCHAR* linetype_name, const double& line_type_scale);

	static Acad::ErrorStatus SetTextHorzMode(const AcDbObjectId& text_id, AcDb::TextHorzMode text_horz_mode);
	static Acad::ErrorStatus SetTextHorzMode(const AcDbObjectIdArray& text_ids, AcDb::TextHorzMode text_horz_mode);

	// entity edit
	static Acad::ErrorStatus MoveEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static Acad::ErrorStatus MoveEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static Acad::ErrorStatus MoveEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const AcGePoint3dArray& target_point);

	static AcDbObjectId CopyEntity(const AcDbObjectId& base_entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static AcDbObjectIdArray CopyEntity(const AcDbObjectIdArray& base_entity_ids, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static AcDbObjectIdArray CopyEntity(const AcDbObjectIdArray& base_entity_ids, const AcGePoint3dArray& base_point, const AcGePoint3dArray& target_point);

	static Acad::ErrorStatus RotateEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& angle, const int& angle_type);
	static Acad::ErrorStatus RotateEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3d& base_point, const double& angle, const int& angle_type);
	static Acad::ErrorStatus RotateEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const double angle[], const int& angle_type);

	static Acad::ErrorStatus DeleteEntity(const AcDbObjectId& entity_id);
	static Acad::ErrorStatus DeleteEntity(const AcDbObjectIdArray& entity_ids);

	static Acad::ErrorStatus ScaleEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& scale);
	static Acad::ErrorStatus ScaleEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3d& base_point, const double& scale);
	static Acad::ErrorStatus ScaleEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const double& scale);

	static AcDbObjectId MirrorEntity(const AcDbObjectId& entity_id, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, const bool need_delete_original_entity);
	static AcDbObjectIdArray MirrorEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, const bool need_delete_original_entity);
};
