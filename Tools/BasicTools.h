#pragma once
#define POLA_DLL
#include "def.h"

/*
*
* Update Time: 2024-12-04
* This is a collection of tools.
* Including mathematical tools and common graphic tools commonly used in the CAD drawing process.
*
*/
class POLADLL BasicTools
{
public:

	// math tools: Angle tools
	static double ConvertAngle(const double& angele, char* target_angle_type);
	static double GetAngleToXaxis(const AcGePoint3d& start_point, const AcGePoint3d& end_point, char* target_angle_type);
	static double GetAngleToXaxis(const AcGeVector3d& vector, char* target_angle_type);
	static double GetAngleByThreePoint(const AcGePoint3d& start_point1, const AcGePoint3d& common_end_point, const AcGePoint3d& start_point2, char* target_angle_type);

	// math tools: Geometry
	static AcGePoint3d GetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static bool IsCollinearPoint(const AcGePoint3d& point1, const AcGePoint3d& point2, const AcGePoint3d& point3);
	static AcGePoint3d AdsPointToPoint3d(const ads_point& ads_point);
	static AcGePoint2d Point3dToPoint2d(const AcGePoint3d& point3d);
	static bool CanDrawRect(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2);
	static double GetDistanceBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static AcGePoint2dArray Point3dToPoint2d(const AcGePoint3dArray& point3d_array);
	static AcGeVector3d GetVectorBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static AcGePoint3d OffsetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance);

	//math tools: Line Tools
	static bool IsIntersectLine(const AcGeLine3d& line_1, const AcGeLine3d& line_2, AcGePoint3d& out_intersect_point);


	// math tools: Numerical tools
	static double Max(const double& num1, const double& num2);
	static double Min(const double& num1, const double& num2);

	// grid tools
	static AcGePoint3dArray DistanceToPointArrayX(AcGePoint3d first_point, double distance[], int cnt);
	static AcGePoint3dArray DistanceToPointArrayY(AcGePoint3d first_point, double distance[], int cnt);
};
