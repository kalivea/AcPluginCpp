#include "StdAfx.h"
#include "DrawEntity.h"

AcDbObjectId DrawEntity::DrawLine(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	AcDbLine* line_pointer = new AcDbLine(start_point, end_point);
	return AddToModelSpace::AddEntityToModelSpace(line_pointer);
}

AcDbObjectId DrawEntity::DrawLine(const AcGePoint3d& start_point, const double& length, const double& angle, char* angle_type)
{
	double temp_angle;
	if (angle_type == "DEG")
		temp_angle = BasicTools::ConvertAngle(angle, "RAD");
	else if (angle_type == "RAD")
		temp_angle = angle;
	else
		throw"Wrong angle type. Only \"DEG\",\"RAD\" can be use!~";

	AcGePoint3d end_point(start_point.x + length * cos(temp_angle), start_point.y + length * sin(temp_angle), 0);
	return DrawLine(start_point, end_point);
}

AcDbObjectId DrawEntity::DrawCircle(const AcGePoint3d& center_point, const double& radius)
{
	AcGeVector3d z_plane_vector(0, 0, 1);
	AcDbCircle* circle_pointer = new AcDbCircle(center_point, z_plane_vector, radius);
	return AddToModelSpace::AddEntityToModelSpace(circle_pointer);
}

AcDbObjectId DrawEntity::DrawCircle(const AcGePoint3d& point1, const AcGePoint3d& point2)
{
	AcGeVector3d z_plane_vector(0, 0, 1);
	AcGePoint3d circle_center = BasicTools::GetMidPoint(point1, point2);
	double radius = BasicTools::GetDistanceBetweenTwoPoint(point1, point2);
	AcDbCircle* circle_pointer = new AcDbCircle(circle_center, z_plane_vector, radius);
	return AddToModelSpace::AddEntityToModelSpace(circle_pointer);
}

AcDbObjectId DrawEntity::DrawCircle(const AcGePoint3d& on_circle_point1, const AcGePoint3d& on_circle_point2, const AcGePoint3d& on_circle_point3)
{
	AcGeCircArc3d arc = AcGeCircArc3d(on_circle_point1, on_circle_point2, on_circle_point3);

	AcGeVector3d z_plane_vector(0, 0, 1);
	AcGePoint3d circle_center = arc.center();
	double radius = arc.radius();
	AcDbCircle* circle_pointer = new AcDbCircle(circle_center, z_plane_vector, radius);
	return AddToModelSpace::AddEntityToModelSpace(circle_pointer);
}

AcDbObjectId DrawEntity::DrawArc(const AcGePoint3d& start_point, const AcGePoint3d& point_on_arc, const AcGePoint3d& end_point)
{
	if (BasicTools::IsCollinearPoint(start_point, point_on_arc, end_point))
	{
		throw"These three points are collinear points, a(n) arc cannot be drawn.";
	}

	AcDbArc* arc_pointer = new AcDbArc();
	AcGeCircArc3d calc_arc(start_point, point_on_arc, end_point);

	AcGeVector3d center_to_start = BasicTools::GetVectorBetweenTwoPoint(calc_arc.center(), start_point);
	AcGeVector3d center_to_end = BasicTools::GetVectorBetweenTwoPoint(calc_arc.center(), end_point);

	arc_pointer->setCenter(calc_arc.center());
	arc_pointer->setRadius(calc_arc.radius());
	arc_pointer->setStartAngle(BasicTools::GetAngleToXaxis(center_to_start, "RAD"));
	arc_pointer->setEndAngle(BasicTools::GetAngleToXaxis(center_to_end, "RAD"));

	return AddToModelSpace::AddEntityToModelSpace(arc_pointer);
}

AcDbObjectId DrawEntity::DrawArc(const AcGePoint3d& center_point, const AcGePoint3d& start_point, const double& angle, char* angle_type)
{
	double temp_angle;
	if (angle_type == "DEG")
		temp_angle = BasicTools::ConvertAngle(angle, "RAD");
	else if (angle_type == "RAD")
		temp_angle = angle;
	else
		throw"Wrong angle type. Only \"DEG\",\"RAD\" can be use!~";

	AcDbArc* arc_pointer = new AcDbArc();

	arc_pointer->setCenter(center_point);
	arc_pointer->setRadius(BasicTools::GetDistanceBetweenTwoPoint(center_point, start_point));

	AcGeVector3d center_to_start = BasicTools::GetVectorBetweenTwoPoint(center_point, start_point);
	arc_pointer->setStartAngle(BasicTools::GetAngleToXaxis(center_to_start, "RAD"));
	arc_pointer->setEndAngle(arc_pointer->startAngle() + temp_angle);

	return AddToModelSpace::AddEntityToModelSpace(arc_pointer);
}

AcDbObjectId DrawEntity::DrawArc(const AcGePoint3d& center_point, const AcGePoint3d& start_point, const AcGePoint3d& end_point, int n)
{
	if (BasicTools::GetDistanceBetweenTwoPoint(center_point, start_point) != BasicTools::GetDistanceBetweenTwoPoint(center_point, end_point))
	{
		throw"The distances from the center point to the start and end points are different.";			//TODO: improve ex handling logic
	}
	return AcDbObjectId();
}

AcDbObjectId DrawEntity::DrawPolyLine(const AcGePoint2dArray& vertices_list, const double& contant_width, bool needClose)
{
	AcDbPolyline* poly_line = new AcDbPolyline();
	if (vertices_list.length() < 2)
	{
		throw"A polyline must consist of at least two vertices.";
	}
	else
	{
		for (int i = 0; i < vertices_list.length(); i++)
		{
			poly_line->addVertexAt(i, vertices_list[i]);
		}
		if (needClose)
			poly_line->setClosed(true);
		else
			poly_line->setClosed(false);
		poly_line->setConstantWidth(contant_width);
	}
	return AddToModelSpace::AddEntityToModelSpace(poly_line);
}

AcDbObjectId DrawEntity::DrawRectangle(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2, const double& width)
{
	if (BasicTools::CanDrawRect(vertex_point1, vertex_point2))
	{
		AcGePoint2dArray rectangle_vertex;
		AcGePoint2d p1(BasicTools::Min(vertex_point1.x, vertex_point2.x), BasicTools::Min(vertex_point1.y, vertex_point2.y));
		AcGePoint2d p2(BasicTools::Max(vertex_point1.x, vertex_point2.x), BasicTools::Min(vertex_point1.y, vertex_point2.y));
		AcGePoint2d p3(BasicTools::Max(vertex_point1.x, vertex_point2.x), BasicTools::Max(vertex_point1.y, vertex_point2.y));
		AcGePoint2d p4(BasicTools::Min(vertex_point1.x, vertex_point2.x), BasicTools::Max(vertex_point1.y, vertex_point2.y));

		rectangle_vertex.append(p1);
		rectangle_vertex.append(p2);
		rectangle_vertex.append(p3);
		rectangle_vertex.append(p4);

		return DrawEntity::DrawPolyLine(rectangle_vertex, width, true);
	}
	else
	{
		return AcDbObjectId::kNull;
	}

}

AcDbObjectId DrawEntity::HatchPattern(char* hatch_pattern_name, const double& pattern_scale, const double& pattern_angle, char* angle_type, bool needAssociative, const AcDbObjectId& entity_id)
{
	return AcDbObjectId();
}

AcDbObjectId DrawEntity::AddText(const AcGePoint3d& insert_position, const TCHAR* insert_text, const AcDbObjectId text_style, const double& text_height, const double& text_rotation)
{
	AcDbText* text_pointer = new AcDbText(insert_position, insert_text, text_style, text_height, text_rotation);
	return AddToModelSpace::AddEntityToModelSpace(text_pointer);
}

AcDbObjectId DrawEntity::AddMtext()
{
	return AcDbObjectId();
}

AcDbObjectId DrawEntity::AlignedDimension(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const AcGePoint3d& on_dimension_line_point, const AcDbObjectId dimension_style, const TCHAR* dimension_text)
{
	AcDbAlignedDimension* aligned_dimension_pointer = new AcDbAlignedDimension(start_point, end_point, on_dimension_line_point, dimension_text, dimension_style);
	return AddToModelSpace::AddEntityToModelSpace(aligned_dimension_pointer);
}

AcDbObjectId DrawEntity::RotatedDimension()
{
	return AcDbObjectId();
}

void DrawEntity::DrawFrame(char* frame_size, const AcGePoint3d& insert_point, const double& scale)
{
	AcDbObjectId frame_outside_id = AcDbObjectId::kNull;
	AcDbObjectId frame_inside_id = AcDbObjectId::kNull;

	AcGePoint3d outside_point1;
	AcGePoint3d outside_point2;
	AcGePoint3d inside_point1;
	AcGePoint3d inside_point2;

	if (frame_size == "A2")
	{
		outside_point1 = insert_point;
		outside_point2 = AcGePoint3d(insert_point.x + 594, insert_point.y + 420, 0);
		inside_point1 = AcGePoint3d(outside_point1.x + 25, outside_point1.y + 10, 0);
		inside_point2 = AcGePoint3d(outside_point2.x - 10, outside_point2.y - 10, 0);

		frame_outside_id = DrawEntity::DrawRectangle(outside_point1, outside_point2, 0);
		frame_inside_id = DrawEntity::DrawRectangle(inside_point1, inside_point2, 0.35);
		EditEntity::ScaleEntity(frame_outside_id, insert_point, scale);
		EditEntity::ScaleEntity(frame_inside_id, insert_point, scale);
		//EditEntity::SetLayer(frame_outside_id, _T("Frame"));
	}
	else if (frame_size == "A2+1/4")
	{
		outside_point1 = insert_point;
		outside_point2 = AcGePoint3d(insert_point.x + 743, insert_point.y + 420, 0);
		inside_point1 = AcGePoint3d(outside_point1.x + 25, outside_point1.y + 10, 0);
		inside_point2 = AcGePoint3d(outside_point2.x - 10, outside_point2.y - 10, 0);

		frame_outside_id = DrawEntity::DrawRectangle(outside_point1, outside_point2, 0);
		frame_inside_id = DrawEntity::DrawRectangle(inside_point1, inside_point2, 0.35);
		EditEntity::ScaleEntity(frame_outside_id, insert_point, scale);
		EditEntity::ScaleEntity(frame_inside_id, insert_point, scale);
	}
	else if (frame_size == "A2+1/2")
	{
		outside_point1 = insert_point;
		outside_point2 = AcGePoint3d(insert_point.x + 891, insert_point.y + 420, 0);
		inside_point1 = AcGePoint3d(outside_point1.x + 25, outside_point1.y + 10, 0);
		inside_point2 = AcGePoint3d(outside_point2.x - 10, outside_point2.y - 10, 0);

		frame_outside_id = DrawEntity::DrawRectangle(outside_point1, outside_point2, 0);
		frame_inside_id = DrawEntity::DrawRectangle(inside_point1, inside_point2, 0.35);
		EditEntity::ScaleEntity(frame_outside_id, insert_point, scale);
		EditEntity::ScaleEntity(frame_inside_id, insert_point, scale);
	}
	else if (frame_size == "A2+3/4")
	{
		outside_point1 = insert_point;
		outside_point2 = AcGePoint3d(insert_point.x + 1040, insert_point.y + 420, 0);
		inside_point1 = AcGePoint3d(outside_point1.x + 25, outside_point1.y + 10, 0);
		inside_point2 = AcGePoint3d(outside_point2.x - 10, outside_point2.y - 10, 0);

		frame_outside_id = DrawEntity::DrawRectangle(outside_point1, outside_point2, 0);
		frame_inside_id = DrawEntity::DrawRectangle(inside_point1, inside_point2, 0.35);
		EditEntity::ScaleEntity(frame_outside_id, insert_point, scale);
		EditEntity::ScaleEntity(frame_inside_id, insert_point, scale);
	}
	else
	{
		acutPrintf(_T("Didn't finished yet T_T\n"));
	}
}

