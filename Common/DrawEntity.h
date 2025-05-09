#pragma once
#include "StdAfx.h"
/*
* Update Time: 2025-01-03
* This is a tool for drawing basic graphics in CAD.
* It can draw commonly used primitives, including straight lines, polylines, arcs, circles, etc.
*
*/

class POLADLL_COMMON_API DrawEntity
{
public:
	static AcDbObjectId DrawLine(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static AcDbObjectId DrawLine(const AcGePoint3d& start_point, const double& length, const double& angle, const int& angle_type);

	static AcDbObjectId DrawCircle(const AcGePoint3d& center_point, const double& radius);
	static AcDbObjectId DrawCircle(const AcGePoint3d& point1, const AcGePoint3d& point2);
	static AcDbObjectId DrawCircle(const AcGePoint3d& on_circle_point1, const AcGePoint3d& on_circle_point2, const AcGePoint3d& on_circle_point3);

	static AcDbObjectId DrawArc(const AcGePoint3d& start_point, const AcGePoint3d& point_on_arc, const AcGePoint3d& end_point);
	static AcDbObjectId DrawArc(const AcGePoint3d& center_point, const AcGePoint3d& start_point, const double& angle, const int angle_type);
	static AcDbObjectId DrawArc(const AcGePoint3d& center_point, const AcGePoint3d& start_point, const AcGePoint3d& end_point, int n);

	static AcDbObjectId DrawPolyLine(const AcGePoint2dArray& vertices_list, const double& contant_width, bool needClose);

	static AcDbObjectId DrawRectangle(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2, const double& width);
	static AcDbObjectId HatchPattern(const TCHAR* hatch_pattern_name, const double& pattern_scale, const double& pattern_angle, const int& angle_type, bool needAssociative, const AcDbObjectIdArray& entity_id_array);

	static AcDbObjectId AddText(const AcGePoint3d& insert_position, const TCHAR* insert_text, const AcDbObjectId text_style = AcDbObjectId::kNull, const double& text_height = 0.2, const double& text_rotation = 0, const double& width_factor = 0.7);
	static AcDbObjectId AddMtext();		//TODO

	static AcDbObjectId AlignedDimension(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const AcGePoint3d& on_dimension_line_point, const AcDbObjectId dimension_style = AcDbObjectId::kNull, const TCHAR* dimension_text = NULL);
	static AcDbObjectId RotatedDimension();		//TODO

	static void DrawFrame(char* frame_size, const AcGePoint3d& insert_point, const double& scale);
	static AcDbObjectId AddMLeader(const AcGePoint3d& insert_point, const AcGePoint3d& point_on_leader, const AcGePoint3d& text_point, const TCHAR* leader_text);
};