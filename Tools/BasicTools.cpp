#include "stdafx.h"
#include "BasicTools.h"


double BasicTools::ConvertAngle(const double& angle, char* target_angle_type)
{
	if (target_angle_type == "RAD")
		return angle * (M_PI / 180.0);
	else if (target_angle_type == "DEG")
		return angle * (180.0 / M_PI);
	else
		throw _T("Wrong angle type. Only \"DEG\",\"RAD\" can be use!~");
}

AcGePoint3d BasicTools::GetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	return AcGePoint3d((start_point.x + end_point.x) / 2.0, (start_point.y + end_point.y) / 2.0, (start_point.z + end_point.z) / 2.0);
}

bool BasicTools::IsCollinearPoint(const AcGePoint3d& point1, const AcGePoint3d& point2, const AcGePoint3d& point3)
{
	AcGeVector3d vector1(point2.x - point1.x, point2.y - point1.y, point2.z - point1.z);
	AcGeVector3d vector2(point2.x - point3.x, point2.y - point3.y, point2.z - point3.z);
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

double BasicTools::GetAngleToXaxis(const AcGePoint3d& start_point, const AcGePoint3d& end_point, char* target_angle_type)
{
	AcGeVector3d i(1.0, 0.0, 0.0);
	AcGeVector3d vector = GetVectorBetweenTwoPoint(start_point, end_point);

	double angle = vector.y >= 0 ? vector.angleTo(i) : -vector.angleTo(i);

	if (target_angle_type == "DEG")
		return BasicTools::ConvertAngle(angle, "DEG");
	else if (target_angle_type == "RAD")
		return angle;
	else
		throw "Wrong angle type. Only \"DEG\",\"RAD\" can be use!~";
}

double BasicTools::GetAngleToXaxis(const AcGeVector3d& vector, char* target_angle_type)
{
	AcGeVector3d i(1.0, 0.0, 0.0);
	double angle = vector.y >= 0 ? vector.angleTo(i) : -vector.angleTo(i);

	if (target_angle_type == "DEG")
		return BasicTools::ConvertAngle(angle, "DEG");
	else if (target_angle_type == "RAD")
		return angle;
	else
		throw "Wrong angle type. Only \"DEG\",\"RAD\" can be use!~";
}

double BasicTools::GetDistanceBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	return sqrt((pow(end_point.x - start_point.x, 2)) + (pow(end_point.y - start_point.y, 2)) + (pow(end_point.z - start_point.z, 2)));
}

double BasicTools::GetAngleByThreePoint(const AcGePoint3d& start_point1, const AcGePoint3d& common_end_point, const AcGePoint3d& start_point2, char* target_angle_type)
{
	AcGeVector3d startPoint1ToEndPoint = BasicTools::GetVectorBetweenTwoPoint(common_end_point, start_point1);
	AcGeVector3d startPoint2ToEndPoint = BasicTools::GetVectorBetweenTwoPoint(common_end_point, start_point2);
	double angle = startPoint1ToEndPoint.angleTo(startPoint2ToEndPoint);

	if (target_angle_type == "DEG")
		return BasicTools::ConvertAngle(angle, "DEG");
	else if (target_angle_type == "RAD")
		return angle;
	else
		throw "Wrong angle type. Only \"DEG\",\"RAD\" can be use!~";
}

AcGePoint3d BasicTools::AdsPointToPoint3d(const ads_point& point)
{
	return AcGePoint3d(point[X], point[Y], point[Z]);
}

AcGePoint2d BasicTools::Point3dToPoint2d(const AcGePoint3d& point3d)
{
	return AcGePoint2d(point3d.x, point3d.y);
}

AcGePoint2dArray BasicTools::Point3dToPoint2d(const AcGePoint3dArray& point3d_array)
{
	AcGePoint2dArray point2d_array;
	for (int i = 0; i < point3d_array.length(); i++)
	{
		point2d_array[i] = Point3dToPoint2d(point3d_array[i]);
	}
	return point2d_array;
}

AcGeVector3d BasicTools::GetVectorBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point)
{
	return AcGeVector3d(end_point.x - start_point.x, end_point.y - start_point.y, end_point.z - start_point.z);
}

bool BasicTools::CanDrawRect(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2)
{
	if (vertex_point1.x != vertex_point2.x && vertex_point1.y != vertex_point2.y)
		return true;
	else
		return false;
}

double BasicTools::Max(const double& num1, const double& num2)
{
	return num1 >= num2 ? num1 : num2;
}

double BasicTools::Min(const double& num1, const double& num2)
{
	return num1 <= num2 ? num1 : num2;
}

AcGePoint3d BasicTools::OffsetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance)
{
	AcGeVector3d vector1 = BasicTools::GetVectorBetweenTwoPoint(start_point, end_point);
	double angle = BasicTools::GetAngleToXaxis(vector1, "RAD");
	AcGePoint3d offseted_start_point(start_point.x - distance * (sin(angle)), start_point.y + distance * (cos(angle)), 0);
	AcGePoint3d offseted_end_point(end_point.x - distance * (sin(angle)), end_point.y + distance * (cos(angle)), 0);
	return BasicTools::GetMidPoint(offseted_start_point, offseted_end_point);
}

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

AcGePoint3dArray BasicTools::DistanceToPointArrayX(AcGePoint3d first_point, double distance[], int cnt)
{
	AcGePoint3dArray temp_array;
	temp_array.append(first_point);
	for (int i = 1; i < cnt; i++)
	{
		temp_array.append(AcGePoint3d(temp_array[i - 1].x + distance[i - 1], temp_array[i - 1].y, temp_array[i - 1].z));
	}
	return temp_array;
}

AcGePoint3dArray BasicTools::DistanceToPointArrayY(AcGePoint3d first_point, double distance[], int cnt)
{
	AcGePoint3dArray temp_array;
	temp_array.append(first_point);
	for (int i = 1; i < cnt; i++)
	{
		temp_array.append(AcGePoint3d(temp_array[i - 1].x, temp_array[i - 1].y + distance[i - 1], temp_array[i - 1].z));
	}
	return temp_array;
}







