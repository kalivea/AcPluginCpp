#pragma once
#include "StdAfx.h"
/*
*
* Update Time: 2025-01-02
* This is a collection of tools.
* Including mathematical tools and common graphic tools commonly used in the CAD drawing process.
*
*/
class POLADLL_TOOLS_API BasicTools
{
public:
	// math tools: Angle tools
	static double ConvertAngle(const double& angle, const int& target_angle_type);
	static double GetAngleToXaxis(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const int& target_angle_type);
	static double GetAngleToXaxis(const AcGeVector3d& vector, const int& target_angle_type);
	static double GetAngleByThreePoint(const AcGePoint3d& start_point1, const AcGePoint3d& common_end_point, const AcGePoint3d& start_point2, const int& target_angle_type);

	// math tools: Geometry
	static AcGePoint3d GetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static bool IsCollinearPoint(const AcGePoint3d& point1, const AcGePoint3d& point2, const AcGePoint3d& point3);
	static AcGePoint3d AdsPointToPoint3d(const ads_point& ads_point);
	static AcGePoint2d Point3dToPoint2d(const AcGePoint3d& point3d);
	static AcGePoint3d Point2dToPoint3d(const AcGePoint2d& point2d);
	static bool CanDrawRect(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2);
	static double GetDistanceBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static AcGePoint2dArray Point3dToPoint2d(const AcGePoint3dArray& point3d_array);
	static AcGeVector3d GetVectorBetweenTwoPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point);
	static AcGePoint3d OffsetMidPoint(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance);
	static bool OffsetPolyLine(const AcDbPolyline& center_line, const double& distance, AcGePoint3dArray& offset_vertex_array);
	static bool OffsetPolyLine(const AcGePoint3dArray& center_array, const double& distance, AcGePoint3dArray& offset_vertex_array);
	static bool OffsetLineSegment(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance, AcGePoint3d out_point[2]);

	// math tools: Rectangle Tools
	static bool IsIntersectRectangle(const AcGePoint3d& vertex_point1, const AcGePoint3d& vertex_point2, const AcGePoint3d& vertex_point3, const AcGePoint3d& vertex_point4);
	static bool IsIntersectRectangle(const AcDbExtents& ext1, const AcDbExtents& ext2);
	static bool IsLineThroughRectangle(const AcGePoint3d& line_point1, const AcGePoint3d& line_point2, const AcGePoint3d vertex_point1, const AcGePoint3d vertex_point2);
	static bool IsPointInRectangle(const AcGePoint3d& point, const AcGePoint3d& vertex_1, const AcGePoint3d& vertex_2);
	static bool IsPointInRectangle(const AcGePoint3d& point, const AcDbExtents& extents);
	
	// math tools: Line Tools
	static bool IsIntersectLine(const AcGeLineSeg3d& line_1, const AcGeLineSeg3d& line_2);
	static bool IsIntersectLine(const AcGePoint3d& line1_point1, const AcGePoint3d& line1_point2, const AcGePoint3d& line2_point1, const AcGePoint3d& line2_point2);
	static AcGePoint3d GetIntersect(const AcGeLineSeg3d& line_1, const AcGeLineSeg3d& line_2);
	static AcGePoint3d GetIntersect(const AcGePoint3d& line1_point1, const AcGePoint3d& line1_point2, const AcGePoint3d& line2_point1, const AcGePoint3d& line2_point2);
	static AcGeLineSeg3d EntityToLineSegment(const AcDbEntity* entity);
	static AcGeLine3d EntityToLine(const AcDbEntity* entity);
	static AcGePoint3d ProjectPointToLineSegment(const AcGePoint3d& point, const AcGeLineSeg3d& line_segment, const AcGeVector3d& project_dirction = AcGeVector3d::kIdentity, const AcGeTol& tol = AcGeContext::gTol);
	static AcGePoint3d ProjectPointToLineSegment(const AcGePoint3d& point, const AcGePoint3d& start_point, const AcGePoint3d& end_point, const AcGeVector3d& project_dirction = AcGeVector3d::kIdentity, const AcGeTol& tol = AcGeContext::gTol);
	
	static double GetDistancePointToLineSegment(const AcGePoint3d& point, const AcGePoint3d& line_segment_start, const AcGePoint3d& line_segment_end);
	static double GetDistancePointToLine(const AcGePoint3d& point, const AcGePoint3d& line_segment_start, const AcGePoint3d& line_segment_end);
	
	// math tools: Numerical tools
	static double Max(const double& num1, const double& num2);
	static double Min(const double& num1, const double& num2);

	// grid tools
	static AcGePoint3dArray DistanceToPointArrayX(const AcGePoint3d& first_point, const double distance[], const int& cnt);
	static AcGePoint3dArray DistanceToPointArrayY(const AcGePoint3d& first_point, const double distance[], const int& cnt);

	// block tools
	static bool IsBlockExist(const TCHAR* block_name);
	static AcDbObjectId GetBlockId(const TCHAR* block_name);

	// database tools
	static AcDbObjectIdArray GetAllEntityIdsInDatabase(const TCHAR* layer_name = NULL, AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase());
};
