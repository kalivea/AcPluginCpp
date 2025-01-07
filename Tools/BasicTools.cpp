#include "StdAfx.h"
#include "BasicTools.h"

/// <summary>
/// Converts the angle to the target angle type.
/// </summary>
/// <param name="angle">Source angel</param>
/// <param name="target_angle_type">0 mean RAD->DEG;1 mean DED->RAD</param>
/// <returns>target angle</returns>
double BasicTools::ConvertAngle(const double& angle, const int target_angle_type)
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
double BasicTools::GetAngleToXaxis(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const int target_angle_type)
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
double BasicTools::GetAngleToXaxis(const AcGeVector3d& vector, const int target_angle_type)
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
double BasicTools::GetAngleByThreePoint(const AcGePoint3d& start_point1, const AcGePoint3d& common_end_point, const AcGePoint3d& start_point2, const int target_angle_type)
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
/// <summary>
/// Determine whether two points intersect.
/// </summary>
/// <param name="line_1"></param>
/// <param name="line_2"></param>
/// <param name="out_intersect_point">if there is an intersection, return a correct intersection coordinate</param>
/// <returns></returns>
bool BasicTools::IsIntersectLine(const AcGeLine3d& line_1, const AcGeLine3d& line_2, AcGePoint3d& out_intersect_point)
{
	if (line_1.intersectWith(line_2, out_intersect_point))
	{
		return true;
	}
	else
	{
		return false;
	}
}
AcGePoint3d BasicTools::GetIntersect(const AcGeLine3d& line_1, const AcGeLine3d& line_2)
{
	AcGePoint3d temp_point;
	if (IsIntersectLine(line_1, line_2, temp_point))
	{
		return temp_point;
	}
	else
	{
		return AcGePoint3d::kOrigin;
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
/// <summary>
/// Grid modules: Calculate X direction point Array based on spacing.
/// </summary>
/// <param name="first_point"></param>
/// <param name="distance"></param>
/// <param name="cnt">Number of spacing</param>
/// <returns></returns>
AcGePoint3dArray BasicTools::DistanceToPointArrayX(const AcGePoint3d& first_point, const double distance[], const int cnt)
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
AcGePoint3dArray BasicTools::DistanceToPointArrayY(const AcGePoint3d& first_point, const double distance[], const int cnt)
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