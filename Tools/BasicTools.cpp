#include "StdAfx.h"
#include "BasicTools.h"
#include "StyleTools.h"

/// <summary>
/// Converts the angle to the target angle type.
/// </summary>
/// <param name="angle">Source angel</param>
/// <param name="target_angle_type">0 mean RAD->DEG;1 mean DED->RAD</param>
/// <returns>target angle</returns>
double BasicTools::ConvertAngle(const double& angle, const int& target_angle_type)
{
	if (target_angle_type == 1)
		return angle * (M_PI / 180.0);
	else if (target_angle_type == 0)
		return angle * (180.0 / M_PI);
	else
		throw;
}
/// <summary>
/// Calculate the midpoint coordinates by passing in the coordinates of the start point and end point.
/// </summary>
/// <param name="start_point"></param>
/// <param name="end_point"></param>
/// <returns>midpoint between two points</returns>
AcGePoint3d BasicTools::GetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	return AcGePoint3d((start_point.x + end_point.x) / 2.0, (start_point.y + end_point.y) / 2.0, (start_point.z + end_point.z) / 2.0);
}
/// <summary>
/// Determine whether three points are collinear.
/// </summary>
/// <param name="point1"></param>
/// <param name="point2"></param>
/// <param name="point3"></param>
/// <returns>result</returns>
bool BasicTools::IsCollinearPoint(const AcGePoint3d& point1, const AcGePoint3d& point2, const AcGePoint3d& point3)
{
	AcGeVector3d vector1 = GetVectorBetweenTwoPoint(point1, point2);
	AcGeVector3d vector2 = GetVectorBetweenTwoPoint(point2, point3);
	double angle = vector1.angleTo(vector2);
	if (angle == 0 || angle == M_PI)
	{
		return true;
	}
	else
	{
		return false;
	}
}
/// <summary>
/// Calculate the angle between the straight line formed by two points and the x-axis.
/// </summary>
/// <param name="start_point"></param>
/// <param name="end_point"></param>
/// <param name="target_angle_type">0 mean target angle type is DEG,1 mean target angle type is RAD</param>
/// <returns>result</returns>
double BasicTools::GetAngleToXaxis(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const int& target_angle_type)
{
	AcGeVector3d i(1.0, 0.0, 0.0);
	AcGeVector3d vector = GetVectorBetweenTwoPoint(start_point, end_point);

	double angle = vector.y >= 0 ? vector.angleTo(i) : -vector.angleTo(i);

	if (target_angle_type == 0)
		return BasicTools::ConvertAngle(angle, 0);
	else if (target_angle_type == 1)
		return angle;
	else
		throw;
}
/// <summary>
/// Calculate the angle between the vector and the x-axis.
/// </summary>
/// <param name="vector"></param>
/// <param name="target_angle_type">0 mean target angle type is DEG,1 mean target angle type is RAD</param>
/// <returns></returns>
double BasicTools::GetAngleToXaxis(const AcGeVector3d& vector, const int& target_angle_type)
{
	AcGeVector3d i(1.0, 0.0, 0.0);
	double angle = vector.y >= 0 ? vector.angleTo(i) : -vector.angleTo(i);

	if (target_angle_type == 0)
		return BasicTools::ConvertAngle(angle, 0);
	else if (target_angle_type == 1)
		return angle;
	else
		throw;
}
/// <summary>
/// Calculate the distance between the two points.
/// </summary>
/// <param name="start_point"></param>
/// <param name="end_point"></param>
/// <returns></returns>
double BasicTools::GetDistanceBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	return start_point.distanceTo(end_point);
}
/// <summary>
/// Calculate the angle formed by three points.
/// </summary>
/// <param name="start_point1"></param>
/// <param name="common_end_point"></param>
/// <param name="start_point2"></param>
/// <param name="target_angle_type">0 mean target angle type is DEG,1 mean target angle type is RAD</param>
/// <returns></returns>
double BasicTools::GetAngleByThreePoint(const AcGePoint3d& start_point1, const AcGePoint3d& common_end_point, const AcGePoint3d& start_point2, const int& target_angle_type)
{
	AcGeVector3d startPoint1ToEndPoint = BasicTools::GetVectorBetweenTwoPoint(common_end_point, start_point1);
	AcGeVector3d startPoint2ToEndPoint = BasicTools::GetVectorBetweenTwoPoint(common_end_point, start_point2);
	double angle = startPoint1ToEndPoint.angleTo(startPoint2ToEndPoint);

	if (target_angle_type == 0)
		return BasicTools::ConvertAngle(angle, 0);
	else if (target_angle_type == 1)
		return angle;
	else
		throw;
}
/// <summary>
/// Convert ads_point type to AcGePoint3d.
/// </summary>
/// <param name="point"></param>
/// <returns></returns>
AcGePoint3d BasicTools::AdsPointToPoint3d(const ads_point& point)
{
	return AcGePoint3d(point[X], point[Y], point[Z]);
}
/// <summary>
/// Convert AcGePoint3d type to AcGePoint2d type.
/// </summary>
/// <param name="point3d"></param>
/// <returns></returns>
AcGePoint2d BasicTools::Point3dToPoint2d(const AcGePoint3d& point3d)
{
	return AcGePoint2d(point3d.x, point3d.y);
}
AcGePoint3d BasicTools::Point2dToPoint3d(const AcGePoint2d& point2d)
{
	return AcGePoint3d(point2d.x, point2d.y, 0);
}
/// <summary>
/// Convert AcGePoint3dArray type to AcGePoint2dArray type.
/// </summary>
/// <param name="point3d"></param>
/// <returns></returns>
AcGePoint2dArray BasicTools::Point3dToPoint2d(const AcGePoint3dArray& point3d_array)
{
	AcGePoint2dArray point2d_array;
	for (int i = 0; i < point3d_array.length(); i++)
	{
		point2d_array[i] = Point3dToPoint2d(point3d_array.at(i));
	}
	return point2d_array;
}
/// <summary>
/// Calculate the vector formed by two points.
/// </summary>
/// <param name="start_point"></param>
/// <param name="end_point"></param>
/// <returns>result vector</returns>
AcGeVector3d BasicTools::GetVectorBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	return end_point.asVector() - start_point.asVector();
}
/// <summary>
/// Confirm whether these two points can draw a rectangle.
/// </summary>
/// <param name="vertex_point1"></param>
/// <param name="vertex_point2"></param>
/// <returns></returns>
bool BasicTools::CanDrawRect(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2)
{
	if (vertex_point1.x != vertex_point2.x && vertex_point1.y != vertex_point2.y)
		return true;
	else
		return false;
}
/// <summary>
/// Get the maximum of two numbers. 
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
double BasicTools::Max(const double& num1, const double& num2)
{
	return num1 >= num2 ? num1 : num2;
}
/// <summary>
/// Get the minimum of two numbers.
/// </summary>
/// <param name="num1"></param>
/// <param name="num2"></param>
/// <returns></returns>
double BasicTools::Min(const double& num1, const double& num2)
{
	return num1 <= num2 ? num1 : num2;
}
/// <summary>
/// Calculate the coordinates of the midpoint of the line segment based on the offset distance
/// </summary>
/// <param name="start_point"></param>
/// <param name="end_point"></param>
/// <param name="distance"></param>
/// <returns></returns>
AcGePoint3d BasicTools::OffsetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance)
{
	AcGeVector3d vector1 = BasicTools::GetVectorBetweenTwoPoint(start_point, end_point);
	double angle = BasicTools::GetAngleToXaxis(vector1, 1);
	AcGePoint3d offseted_start_point(start_point.x - distance * (sin(angle)), start_point.y + distance * (cos(angle)), 0);
	AcGePoint3d offseted_end_point(end_point.x - distance * (sin(angle)), end_point.y + distance * (cos(angle)), 0);
	return BasicTools::GetMidPoint(offseted_start_point, offseted_end_point);
}

bool BasicTools::OffsetPolyLine(const AcDbPolyline& center_line, const double& distance, AcGePoint3dArray& offset_vertex_array)
{
	AcDbVoidPtrArray offset_curves;
	Acad::ErrorStatus error_status;
	error_status = center_line.getOffsetCurves(distance, offset_curves);
	if (error_status == Acad::eOk)
	{
		if (offset_curves.length() == 1)			// Currently only polylines can be converted!
		{
			AcDbPolyline* pl = AcDbPolyline::cast(reinterpret_cast<const AcRxObject*> (offset_curves.at(0)));	// TODO: unsafely cast!!
			for (unsigned int i = 0; i < pl->numVerts(); i++)
			{
				AcGePoint2d temp;
				pl->getPointAt(i, temp);
				offset_vertex_array.append(BasicTools::Point2dToPoint3d(temp));
			}
			delete pl;
		}
		else
		{
			throw;
		}
	}
	return true;
}

bool BasicTools::OffsetPolyLine(const AcGePoint3dArray& center_array, const double& distance, AcGePoint3dArray& offset_vertex_array)
{
	AcDbPolyline* poly_line = new AcDbPolyline();
	for (int i = 0;i < center_array.length();i++)
	{
		poly_line->addVertexAt(i, BasicTools::Point3dToPoint2d(center_array.at(i)));
	}
	return OffsetPolyLine(*poly_line, distance, offset_vertex_array);
}

bool BasicTools::OffsetLineSegment(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance, AcGePoint3d out_point[2])
{
	AcGePoint3dArray temp_center_array;
	AcGePoint3dArray temp_offset_array;

	temp_center_array.append(start_point);
	temp_center_array.append(end_point);

	if (OffsetPolyLine(temp_center_array, distance, temp_offset_array))
	{
		out_point[0] = temp_offset_array.at(0);
		out_point[1] = temp_offset_array.at(1);
		return true;
	}
	else
	{
		throw;
	}
	return false;
}

bool BasicTools::IsIntersectRectangle(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2, const AcGePoint3d& vertex_point3, const AcGePoint3d& vertex_point4)
{
	double rect1_min_x = BasicTools::Min(vertex_point1.x, vertex_point2.x);
	double rect1_max_x = BasicTools::Max(vertex_point1.x, vertex_point2.x);
	double rect1_min_y = BasicTools::Min(vertex_point1.y, vertex_point2.y);
	double rect1_max_y = BasicTools::Max(vertex_point1.y, vertex_point2.y);

	double rect2_min_x = BasicTools::Min(vertex_point3.x, vertex_point4.x);
	double rect2_max_x = BasicTools::Max(vertex_point3.x, vertex_point4.x);
	double rect2_min_y = BasicTools::Min(vertex_point3.y, vertex_point4.y);
	double rect2_max_y = BasicTools::Max(vertex_point3.y, vertex_point4.y);

	if (rect1_max_x < rect2_min_x || rect1_min_x > rect2_max_x || rect1_max_y < rect2_min_y || rect1_min_y > rect2_max_y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BasicTools::IsIntersectRectangle(const AcDbExtents& ext1, const AcDbExtents& ext2)
{
	const AcGePoint3d& min1 = ext1.minPoint();
	const AcGePoint3d& max1 = ext1.maxPoint();
	const AcGePoint3d& min2 = ext2.minPoint();
	const AcGePoint3d& max2 = ext2.maxPoint();

	return IsIntersectRectangle(min1, max1, min2, max2);
}

bool BasicTools::IsLineThroughRectangle(const AcGePoint3d& line_point1, const AcGePoint3d& line_point2, const AcGePoint3d vertex_point1, const AcGePoint3d vertex_point2)
{
	if (!CanDrawRect(vertex_point1, vertex_point2))
	{
		throw;
	}

	AcGePoint3dArray rectangle_vertex;
	AcGePoint3d p1(BasicTools::Min(vertex_point1.x, vertex_point2.x), BasicTools::Min(vertex_point1.y, vertex_point2.y), 0);
	AcGePoint3d p2(BasicTools::Max(vertex_point1.x, vertex_point2.x), BasicTools::Min(vertex_point1.y, vertex_point2.y), 0);
	AcGePoint3d p3(BasicTools::Max(vertex_point1.x, vertex_point2.x), BasicTools::Max(vertex_point1.y, vertex_point2.y), 0);
	AcGePoint3d p4(BasicTools::Min(vertex_point1.x, vertex_point2.x), BasicTools::Max(vertex_point1.y, vertex_point2.y), 0);

	rectangle_vertex.append(p1);
	rectangle_vertex.append(p2);
	rectangle_vertex.append(p3);
	rectangle_vertex.append(p4);

	bool first = IsIntersectLine(rectangle_vertex.at(1), rectangle_vertex.at(3), line_point1, line_point2);
	bool second = IsIntersectLine(rectangle_vertex.at(0), rectangle_vertex.at(2), line_point1, line_point2);

	if (first || second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

AcDbObjectIdArray BasicTools::GetAllEntityIdsInDatabase(const TCHAR* layer_name, AcDbDatabase* database)
{
	AcDbObjectIdArray all_entity_ids;
	bool needFilterLayer;
	if ((layer_name != NULL ? true : false) && (StyleTools::IsLayerExist(layer_name)))
	{
		needFilterLayer = true;
	}
	else
	{
		needFilterLayer = false;
	}
	AcDbObjectId layer_id = AcDbObjectId::kNull;

	AcDbBlockTable* block_table = nullptr;
	database->getBlockTable(block_table, OpenMode::kForRead);
	AcDbBlockTableRecord* block_table_record = nullptr;
	block_table->getAt(ACDB_MODEL_SPACE, block_table_record, OpenMode::kForRead);

	AcDbBlockTableRecordIterator* block_table_record_iterator = nullptr;
	block_table_record->newIterator(block_table_record_iterator);
	for (block_table_record_iterator->start(); !block_table_record_iterator->done(); block_table_record_iterator->step())
	{
		AcDbEntity* entity = nullptr;
		Acad::ErrorStatus error_status;
		error_status = block_table_record_iterator->getEntity(entity, AcDb::kForRead);
		if (error_status == Acad::eOk)
		{
			if (needFilterLayer)
			{
				if (entity->layerId() == layer_id)
				{
					all_entity_ids.append(entity->objectId());
				}
			}
			else
			{
				all_entity_ids.append(entity->objectId());
			}
			entity->close();
		}
		else
		{
			throw;
		}
	}
	delete block_table_record_iterator;
	block_table_record->close();
	block_table->close();

	return all_entity_ids;
}

/// <summary>
/// Determine whether two points intersect.
/// </summary>
/// <param name="line_1"></param>
/// <param name="line_2"></param>
/// <param name="out_intersect_point">if there is an intersection, return a correct intersection coordinate</param>
/// <returns></returns>
bool BasicTools::IsIntersectLine(const AcGeLineSeg3d& line_1, const AcGeLineSeg3d& line_2)
{
	AcGePoint3d out_intersect_point(6496, 6496, 6496);
	if (line_1.intersectWith(line_2, out_intersect_point))
	{
		if (line_1.isOn(out_intersect_point) && line_2.isOn(out_intersect_point))
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool BasicTools::IsIntersectLine(const AcGePoint3d& line1_point1, const AcGePoint3d& line1_point2, const AcGePoint3d& line2_point1, const AcGePoint3d& line2_point2)
{
	AcGeLineSeg3d line1(line1_point1, line1_point2);
	AcGeLineSeg3d line2(line2_point1, line2_point2);
	return IsIntersectLine(line1, line2);
}

AcGePoint3d BasicTools::GetIntersect(const AcGeLineSeg3d& line_1, const AcGeLineSeg3d& line_2)
{
	AcGePoint3d temp_point;
	if (IsIntersectLine(line_1, line_2))
	{
		line_1.intersectWith(line_2, temp_point);
		return temp_point;
	}
	else
	{
		return AcGePoint3d(6496, 6496, 6496);
	}
}

AcGePoint3d BasicTools::GetIntersect(const AcGePoint3d& line1_point1, const AcGePoint3d& line1_point2, const AcGePoint3d& line2_point1, const AcGePoint3d& line2_point2)
{
	AcGeLineSeg3d line1(line1_point1, line1_point2);
	AcGeLineSeg3d line2(line2_point1, line2_point2);
	return GetIntersect(line1, line2);
}

AcGeLineSeg3d BasicTools::EntityToLineSegment(const AcDbEntity* entity)
{
	if (entity->isKindOf(AcDbLine::desc()))
	{
		AcDbLine* line = AcDbLine::cast(entity);
		AcGePoint3d start_point = line->startPoint();
		AcGePoint3d end_point = line->endPoint();

		line->close();
		return AcGeLineSeg3d(start_point, end_point);
	}
	else
	{
		throw;
	}
}

AcGeLine3d BasicTools::EntityToLine(const AcDbEntity* entity)
{
	if (entity->isKindOf(AcDbLine::desc()))
	{
		AcDbLine* line = AcDbLine::cast(entity);
		AcGePoint3d start_point = line->startPoint();
		AcGePoint3d end_point = line->endPoint();

		line->close();
		return AcGeLine3d(start_point, end_point);
	}
	else
	{
		throw;
	}
}

AcGePoint3d BasicTools::ProjectPointToLineSegment(const AcGePoint3d& point, const AcGeLineSeg3d& line_segment, const AcGeVector3d& project_dirction, const AcGeTol& tol)
{
	AcGePoint3d project_point = line_segment.projClosestPointTo(point, project_dirction, tol);
	return project_point;
}

AcGePoint3d BasicTools::ProjectPointToLineSegment(const AcGePoint3d& point, const AcGePoint3d& start_point, const AcGePoint3d& end_point, const AcGeVector3d& project_dirction, const AcGeTol& tol)
{
	AcGeLineSeg3d line_segment(start_point, end_point);
	return ProjectPointToLineSegment(point, line_segment, project_dirction, tol);
}

/// <summary>
/// Grid modules: Calculate X direction point Array based on spacing.
/// </summary>
/// <param name="first_point"></param>
/// <param name="distance"></param>
/// <param name="cnt">Number of spacing</param>
/// <returns></returns>
AcGePoint3dArray BasicTools::DistanceToPointArrayX(const AcGePoint3d& first_point, const double distance[], const int& cnt)
{
	AcGePoint3dArray temp_array;
	temp_array.append(first_point);
	for (int i = 1; i < cnt; i++)
	{
		temp_array.append(AcGePoint3d(temp_array[i - 1].x + distance[i - 1], temp_array[i - 1].y, temp_array[i - 1].z));
	}
	return temp_array;
}
/// <summary>
/// Grid modules: Calculate Y direction point Array based on spacing.
/// </summary>
/// <param name="first_point"></param>
/// <param name="distance"></param>
/// <param name="cnt">Number of spacing</param>
/// <returns></returns>
AcGePoint3dArray BasicTools::DistanceToPointArrayY(const AcGePoint3d& first_point, const double distance[], const int& cnt)
{
	AcGePoint3dArray temp_array;
	temp_array.append(first_point);
	for (int i = 1; i < cnt; i++)
	{
		temp_array.append(AcGePoint3d(temp_array[i - 1].x, temp_array[i - 1].y + distance[i - 1], temp_array[i - 1].z));
	}
	return temp_array;
}

bool BasicTools::IsBlockExist(const TCHAR* block_name)
{
	AcDbBlockTable* block_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(block_table, OpenMode::kForRead);
	if (block_table->has(block_name))
	{
		block_table->close();
		return true;
	}
	else
	{
		block_table->close();
		return false;
	}
}

AcDbObjectId BasicTools::GetBlockId(const TCHAR* block_name)
{
	AcDbBlockTable* block_table = nullptr;
	AcDbObjectId block_object_id = AcDbObjectId::kNull;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(block_table, OpenMode::kForRead);
	if (IsBlockExist(block_name))
	{
		block_table->getAt(block_name, block_object_id);
		block_table->close();
	}
	else
	{
		block_table->close();
	}
	return block_object_id;
}
test;