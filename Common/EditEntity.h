#pragma once
/*
* Update Time: 2024-12-04
* This is a collection of tools that can edit CAD entity(s).
* It can scale, move, copy, etc CAD entiy(s).
*
*/
class POLADLL_COMMON_API EditEntity
{
public:
	// property edit
	static void SetColor(const AcDbObjectId& entity_id, const unsigned char& color_index);
	static void SetColor(const AcDbObjectIdArray& entity_ids, const unsigned char& color_index);
	static void SetColor(const AcDbObjectIdArray& entity_id, const unsigned char color_index[]);

	static void SetLayer(const AcDbObjectId& entity_id, const TCHAR* layer_name);
	static void SetLayer(const AcDbObjectIdArray& entity_ids, const TCHAR* layer_name);
	static void SetLayer(const AcDbObjectIdArray& entity_ids, const TCHAR* layer_name[]);

	static void SetLinetype(const AcDbObjectId& entity_id, const TCHAR* linetype_name, const double& line_type_scale);
	static void SetLinetype(const AcDbObjectIdArray& entity_ids, const TCHAR* linetype_name, const double& line_type_scale);

	// entity edit
	static void MoveEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static void MoveEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const AcGePoint3dArray& target_point);

	static AcDbObjectId CopyEntity(const AcDbObjectId& base_entity_id, const AcGePoint3d& base_point, const AcGePoint3d& target_point);
	static AcDbObjectIdArray CopyEntity(const AcDbObjectIdArray& base_entity_ids, const AcGePoint3dArray& base_point, const AcGePoint3dArray& target_point);

	static void RotateEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& angle, char* angle_type);

	static void DeleteEntity(const AcDbObjectId& entity_id);
	static void DeleteEntity(const AcDbObjectIdArray& entity_ids);

	static void ScaleEntity(const AcDbObjectId& entity_id, const AcGePoint3d& base_point, const double& scale);
	static void ScaleEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3dArray& base_point, const double& scale);

	static AcDbObjectId MirrorEntity(const AcDbObjectId& entity_id, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, const bool need_delete_original_entity);
	static AcDbObjectIdArray MirrorEntity(const AcDbObjectIdArray& entity_ids, const AcGePoint3d& mirror_point1, const AcGePoint3d& mirror_point2, const bool need_delete_original_entity);
};

