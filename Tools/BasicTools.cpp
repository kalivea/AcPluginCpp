#include "StdAfx.h"
#include "BasicTools.h"
#include "StyleTools.h"
#include <cmath>
#include <limits>
#include <new>
#include "../Common/DbObjectOwner.h"

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
		return std::numeric_limits<double>::quiet_NaN();
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

AcGePoint3d BasicTools::GetMidPoint(const AcDbExtents& extents)
{
	return GetMidPoint(extents.minPoint(), extents.maxPoint());
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
		return std::numeric_limits<double>::quiet_NaN();
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
		return std::numeric_limits<double>::quiet_NaN();
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
		return std::numeric_limits<double>::quiet_NaN();
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
		point2d_array.append(Point3dToPoint2d(point3d_array.at(i)));
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

int BasicTools::RandomInt(const int& min, const int& max)
{
	unsigned seed = static_cast<unsigned> (std::chrono::system_clock::now().time_since_epoch().count());
	std::mt19937 random_generator(seed);
	std::uniform_int_distribution<> int_distribution(min, max);
	return int_distribution(random_generator);
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

// TOOLS_OFFSET_TEST_BEGIN
namespace {
struct OffsetCurveCleanup {
    AcDbVoidPtrArray& curves;
    ~OffsetCurveCleanup() {
        for (int i = 0; i < curves.length(); ++i) {
            // The SDK outputs transient entities; a cast remains a borrowed alias.
            delete static_cast<AcDbEntity*>(curves[i]);
        }
    }
};
bool ToolsFinitePoint(const AcGePoint3d& point) {
    return std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z);
}
bool ToolsValidLineEndpoints(const AcGePoint3d& start, const AcGePoint3d& end) {
    return ToolsFinitePoint(start) && ToolsFinitePoint(end) && !start.isEqualTo(end) &&
        std::isfinite(end.x - start.x) && std::isfinite(end.y - start.y) && std::isfinite(end.z - start.z);
}
}
Acad::ErrorStatus BasicTools::OffsetPolyLineChecked(const AcDbPolyline& center_line, const double& distance, AcGePoint3dArray& output)
{
    try {
        output.removeAll();
        if (!std::isfinite(distance) || distance == 0.0 || center_line.numVerts() < 2)
            return Acad::eInvalidInput;
        AcDbVoidPtrArray curves;
        // This guard requires no allocation, and also covers partial SDK outputs
        // and failures allocating the copied point array.
        OffsetCurveCleanup cleanup{curves};
        Acad::ErrorStatus status = center_line.getOffsetCurves(distance, curves);
        if (status != Acad::eOk) return status;
        // A flattened collection of disconnected curves cannot represent one beam.
        if (curves.length() != 1) return Acad::eInvalidInput;
        const AcDbPolyline* offset = AcDbPolyline::cast(static_cast<AcDbEntity*>(curves[0]));
        if (!offset || offset->numVerts() < 2) return Acad::eInvalidInput;
        AcGePoint3dArray points;
        for (unsigned int i = 0; i < offset->numVerts(); ++i)
        {
            AcGePoint3d point;
            status = offset->getPointAt(i, point);
            if (status != Acad::eOk) return status;
            if (!ToolsFinitePoint(point)) return Acad::eInvalidInput;
            points.append(point);
        }
        if (offset->isClosed() && !points.first().isEqualTo(points.last()))
            points.append(points.first());
        output = points;
        return Acad::eOk;
    } catch (const std::bad_alloc&) {
        output.removeAll();
        return Acad::eOutOfMemory;
    }
}

Acad::ErrorStatus BasicTools::OffsetPolyLineChecked(const AcGePoint3dArray& center, const double& distance, AcGePoint3dArray& output)
{
    try {
        // Snapshot aliased input before clearing the required output parameter.
        AcGePoint3dArray points = center;
        output.removeAll();
        if (!std::isfinite(distance) || distance == 0.0 || points.length() < 2) return Acad::eInvalidInput;
        AcDbPolyline line;
        for (int i = 0; i < points.length(); ++i)
        {
            if (!ToolsFinitePoint(points[i])) return Acad::eInvalidInput;
            if (i > 0 && !ToolsValidLineEndpoints(points[i - 1], points[i])) return Acad::eInvalidInput;
            Acad::ErrorStatus status = line.addVertexAt(i, Point3dToPoint2d(points[i]));
            if (status != Acad::eOk) return status;
        }
        return OffsetPolyLineChecked(line, distance, output);
    } catch (const std::bad_alloc&) {
        output.removeAll();
        return Acad::eOutOfMemory;
    }
}
// TOOLS_OFFSET_TEST_END

bool BasicTools::OffsetPolyLine(const AcDbPolyline& line, const double& distance, AcGePoint3dArray& output)
{
    return OffsetPolyLineChecked(line, distance, output) == Acad::eOk;
}
bool BasicTools::OffsetPolyLine(const AcGePoint3dArray& points, const double& distance, AcGePoint3dArray& output)
{
    return OffsetPolyLineChecked(points, distance, output) == Acad::eOk;
}

bool BasicTools::OffsetLineSegment(const AcGePoint3d& start_point, const AcGePoint3d& end_point, const double& distance, AcGePoint3d out_point[2])
{
	AcGePoint3dArray temp_center_array;
	AcGePoint3dArray temp_offset_array;

	temp_center_array.append(start_point);
	temp_center_array.append(end_point);

	if (out_point && OffsetPolyLine(temp_center_array, distance, temp_offset_array) && temp_offset_array.length() == 2)
	{
		out_point[0] = temp_offset_array.at(0);
		out_point[1] = temp_offset_array.at(1);
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool BasicTools::OffsetLineSegment(const AcGeLineSeg3d& line_segment, const double& distance, AcGePoint3d out_point[2])
{
	return OffsetLineSegment(line_segment.startPoint(), line_segment.endPoint(), distance, out_point);
}

bool BasicTools::IsLeftPoint(const AcGePoint3d& point1, const AcGePoint3d& point2)
{
	return point1.x < point2.x ? true : false;
}

bool BasicTools::SortPointFromLeftToRight(const AcGePoint3dArray& source_point_array, AcGePoint3dArray& target_point_array)
{
    std::vector<AcGePoint3d> points;
    for (int i = 0; i < source_point_array.length(); ++i) {
        if (!ToolsFinitePoint(source_point_array[i])) { target_point_array.removeAll(); return false; }
        points.push_back(source_point_array[i]);
    }
    target_point_array.removeAll();
    if (points.empty()) return false;
    std::sort(points.begin(), points.end(), &BasicTools::IsLeftPoint);
    for (const AcGePoint3d& point : points) target_point_array.append(point);
    return true;
}

bool BasicTools::IsBottomPoint(const AcGePoint3d& point1, const AcGePoint3d& point2)
{
	return point1.y < point2.y ? true : false;
}

bool BasicTools::SortPointFromBottomToTop(const AcGePoint3dArray& source_point_array, AcGePoint3dArray& target_point_array)
{
    std::vector<AcGePoint3d> points;
    for (int i = 0; i < source_point_array.length(); ++i) {
        if (!ToolsFinitePoint(source_point_array[i])) { target_point_array.removeAll(); return false; }
        points.push_back(source_point_array[i]);
    }
    target_point_array.removeAll();
    if (points.empty()) return false;
    std::sort(points.begin(), points.end(), &BasicTools::IsBottomPoint);
    for (const AcGePoint3d& point : points) target_point_array.append(point);
    return true;
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
		return false;
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

bool BasicTools::IsPointInRectangle(const AcGePoint3d& point, const AcGePoint3d& vertex_1, const AcGePoint3d& vertex_2)
{
	double min_X = BasicTools::Min(vertex_1.x, vertex_2.x);
	double max_X = BasicTools::Max(vertex_1.x, vertex_2.x);
	double min_Y = BasicTools::Min(vertex_1.y, vertex_2.y);
	double max_Y = BasicTools::Max(vertex_1.y, vertex_2.y);

	return (point.x >= min_X && point.x <= max_X) && (point.y >= min_Y && point.y <= max_Y);
}

bool BasicTools::IsPointInRectangle(const AcGePoint3d& point, const AcDbExtents& extents)
{
	return IsPointInRectangle(point, extents.maxPoint(), extents.minPoint());
}

Acad::ErrorStatus BasicTools::GetAllEntityIdsInDatabase(AcDbObjectIdArray& ids, const TCHAR* layer_name, AcDbDatabase* database)
{
    ids.removeAll();
    if (!database) return Acad::eNoDatabase;
    AcDbObjectId layer_id;
    if (layer_name)
    {
        AcDbLayerTable* raw = nullptr;
        Acad::ErrorStatus status = database->getLayerTable(raw, AcDb::kForRead);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbLayerTable> layers(raw);
        status = layers->getAt(layer_name, layer_id);
        status = Pola::Close(layers, status);
        if (status != Acad::eOk) return status;
    }
    AcDbBlockTable* raw_table = nullptr;
    Acad::ErrorStatus status = database->getBlockTable(raw_table, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTable> table(raw_table);
    AcDbBlockTableRecord* raw_record = nullptr;
    status = table->getAt(ACDB_MODEL_SPACE, raw_record, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTableRecord> record(raw_record);
    AcDbBlockTableRecordIterator* raw_iterator = nullptr;
    status = record->newIterator(raw_iterator);
    if (status != Acad::eOk) return status;
    std::unique_ptr<AcDbBlockTableRecordIterator> iterator(raw_iterator);
    AcDbObjectIdArray result;
    for (; !iterator->done(); iterator->step())
    {
        AcDbEntity* raw_entity = nullptr;
        status = iterator->getEntity(raw_entity, AcDb::kForRead);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbEntity> entity(raw_entity);
        if (!layer_name || entity->layerId() == layer_id) result.append(entity->objectId());
        status = Pola::Close(entity);
        if (status != Acad::eOk) return status;
    }
    iterator.reset();
    status = Pola::Close(record);
    status = Pola::Close(table, status);
    if (status == Acad::eOk) ids = result;
    return status;
}

AcDbObjectIdArray BasicTools::GetAllEntityIdsInDatabase(const TCHAR* layer_name, AcDbDatabase* database)
{
    AcDbObjectIdArray ids;
    GetAllEntityIdsInDatabase(ids, layer_name, database);
    return ids;
}

std::vector<int> BasicTools::CalculateReinforcement(int beam_width, int bar_diameter, int bars_num, int cover_thickness, int stirrup_diameter)
{
	int effective_width = beam_width - 2 * cover_thickness - 2 * stirrup_diameter;

	int min_spacing_top = (std::max)(30, static_cast<int>(1.5 * bar_diameter));
	int min_spacing_bottom = (std::max)(25, bar_diameter);

	int max_bars_per_row = (effective_width + min_spacing_top) / (bar_diameter + min_spacing_top);

	if (max_bars_per_row < 1)
	{
		max_bars_per_row = 1;
	}
	int rows = static_cast<int>(std::ceil(static_cast<double>(bars_num) / max_bars_per_row));
	std::vector<int> bars_in_row(rows, 0);
	int remaining_bars = bars_num;

	for (int i = 0; i < rows; i++)
	{
		if (remaining_bars >= max_bars_per_row)
		{
			bars_in_row[i] = max_bars_per_row;
			remaining_bars -= max_bars_per_row;
		}
		else
		{
			bars_in_row[i] = remaining_bars;
			remaining_bars = 0;
		}
	}
	return bars_in_row;
}

int BasicTools::CalculateMaxBarsPerRow(int beam_width, int bar_diameter, int cover_thickness, int stirrup_diameter, int min_spacing_required, double spacing_factor)
{
	int effective_width = beam_width - 2 * cover_thickness - 2 * stirrup_diameter;
	int min_spacing = (std::max)(min_spacing_required, static_cast<int>(spacing_factor * bar_diameter));

	int max_bars_per_row = (effective_width + min_spacing_required) / (bar_diameter + min_spacing_required);
	return (std::max)(1, max_bars_per_row);
}

int BasicTools::CalculateMaxBars(int beam_width, int bar_diameter, int cover_thickness, int stirrup_diameter, int min_spacing, double spacing_factor)
{
	int max_bars_per_row = CalculateMaxBarsPerRow(beam_width, bar_diameter, cover_thickness, stirrup_diameter, min_spacing, spacing_factor);
	int max_rows = 3;
	return max_bars_per_row * max_rows;
}

// string tools
std::basic_string<TCHAR> BasicTools::EscapeBackslashes(const TCHAR* path)
{
	std::basic_string<TCHAR> escaped;
	if (!path) return escaped;
	for (size_t i = 0; path[i] != _T('\0'); i++)
	{
		if (path[i] == _T('\\'))
			escaped += _T("\\\\");
		else
			escaped += path[i];
	}
	return escaped;
}

TCHAR* BasicTools::StringToChar(CString& str)
{
	int len = str.GetLength();
	TCHAR* tr = str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr;
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

// TOOLS_BORROWED_LINE_TEST_BEGIN
Acad::ErrorStatus BasicTools::EntityToLineSegment(const AcDbEntity* entity, AcGeLineSeg3d& segment)
{
    segment = AcGeLineSeg3d();
    const AcDbLine* line = entity ? AcDbLine::cast(entity) : nullptr;
    if (!line || !ToolsValidLineEndpoints(line->startPoint(), line->endPoint())) return Acad::eInvalidInput;
    segment.set(line->startPoint(), line->endPoint());
    return Acad::eOk;
}
Acad::ErrorStatus BasicTools::EntityToLine(const AcDbEntity* entity, AcGeLine3d& result)
{
    result = AcGeLine3d();
    const AcDbLine* line = entity ? AcDbLine::cast(entity) : nullptr;
    if (!line || !ToolsValidLineEndpoints(line->startPoint(), line->endPoint())) return Acad::eInvalidInput;
    result.set(line->startPoint(), line->endPoint());
    return Acad::eOk;
}
AcGeLineSeg3d BasicTools::EntityToLineSegment(const AcDbEntity* entity)
{
    AcGeLineSeg3d segment;
    EntityToLineSegment(entity, segment);
    return segment;
}
AcGeLine3d BasicTools::EntityToLine(const AcDbEntity* entity)
{
    AcGeLine3d line;
    EntityToLine(entity, line);
    return line;
}

// TOOLS_BORROWED_LINE_TEST_END

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

double BasicTools::GetDistancePointToLineSegment(const AcGePoint3d& point, const AcGePoint3d& line_segment_start, const AcGePoint3d& line_segment_end)
{
	AcGeLineSeg3d line(line_segment_start, line_segment_end);
	return line.distanceTo(point);
}

double BasicTools::GetDistancePointToLineSegment(const AcGePoint3d& point, const AcGeLineSeg3d& line_segment)
{
	AcGePoint3d start_point = line_segment.startPoint();
	AcGePoint3d end_point = line_segment.endPoint();
	return GetDistancePointToLineSegment(point, start_point, end_point);
}

double BasicTools::GetDistancePointToLine(const AcGePoint3d& point, const AcGePoint3d& line_start, const AcGePoint3d& line_end)
{
	AcGeLine3d line(line_start, line_end);
	return line.distanceTo(point);
}

bool BasicTools::IsParallelLineSeg(const AcGePoint3d& line1_start, const AcGePoint3d& line1_end, const AcGePoint3d& line2_start, const AcGePoint3d& line2_end)
{
	AcGeLineSeg3d line1(line1_start, line1_end);
	AcGeLineSeg3d line2(line2_start, line2_end);

	return line1.isParallelTo(line2);
}

bool BasicTools::IsParallelLineSeg(const AcGeLineSeg3d& line1, const AcGeLineSeg3d& line2)
{
	return line1.isParallelTo(line2);
}

bool BasicTools::IsParallelLine(const AcGePoint3d& line1_start, const AcGePoint3d& line1_end, const AcGePoint3d& line2_start, const AcGePoint3d& line2_end)
{
	AcGeLine3d line1(line1_start, line1_end);
	AcGeLine3d line2(line2_start, line2_end);

	return line1.isParallelTo(line2);
}

AcGePoint3dArray BasicTools::GetPolyLineIntersections(const AcDbPolyline& line1, const AcDbPolyline& line2)
{
	AcGePoint3dArray intersection_points;

	if (!line1.isKindOf(AcDbPolyline::desc()) || !line2.isKindOf(AcDbPolyline::desc()))
	{
		return intersection_points;
	}

	AcDbCurve* curve1 = const_cast<AcDbPolyline*>(&line1);
	AcDbCurve* curve2 = const_cast<AcDbPolyline*>(&line2);

	AcGePoint3dArray points;
	AcDb::Intersect intersetct_type = AcDb::kOnBothOperands;
	Acad::ErrorStatus es = curve1->intersectWith(curve2, intersetct_type, points);

	if (es == Acad::eOk)
	{
		intersection_points = points;
	}
	return intersection_points;
}

AcGePoint3dArray BasicTools::GetPolyLineIntersections(const AcGePoint3dArray& line1_vertex, const AcGePoint3dArray& line2_vertex)
{
	AcDbObjectPointer<AcDbPolyline> line1;
	AcDbObjectPointer<AcDbPolyline> line2;

	line1.create();
	line2.create();
	for (int i = 0;i < line1_vertex.length();i++)
	{
		line1->addVertexAt(i, BasicTools::Point3dToPoint2d(line1_vertex.at(i)));
	}
	for (int i = 0;i < line2_vertex.length();i++)
	{
		line2->addVertexAt(i, BasicTools::Point3dToPoint2d(line2_vertex.at(i)));
	}
	return GetPolyLineIntersections(*line1, *line2);
}

AcGePoint3dArray BasicTools::GetPolylineVertex(const AcDbPolyline& poly_line)
{
	AcGePoint3dArray vertex_array;
	for (int i = 0;i < static_cast<int>(poly_line.numVerts());i++)
	{
		AcGePoint2d point;
		poly_line.getPointAt(i, point);
		vertex_array.append(BasicTools::Point2dToPoint3d(point));
	}
	return vertex_array;
}

AcGePoint3d BasicTools::GetArcMidPoint(AcGeCircArc3d& arc)
{
	AcGePoint3d center = arc.center();
	double radius = arc.radius();
	double start_angle = arc.startAng();
	double end_angle = arc.endAng();
	AcGeVector3d normal = arc.normal();

	bool isClockwise = (normal.z < 0.0);

	double delta_theta;

	if (std::abs(start_angle - end_angle) < AcGeContext::gTol.equalPoint())
		delta_theta = 2 * M_PI;
	else
	{
		if (isClockwise)
			delta_theta = std::fmod(start_angle - end_angle + 2 * M_PI, 2 * M_PI);
		else
			delta_theta = std::fmod(end_angle - start_angle + 2 * M_PI, 2 * M_PI);
	}

	double mid_angle;
	if (isClockwise)
		mid_angle = start_angle - delta_theta / 2.0;
	else
		mid_angle = start_angle + delta_theta / 2.0;

	mid_angle = std::fmod(mid_angle + 2 * M_PI, 2 * M_PI);

	double x = center.x + radius * std::cos(mid_angle);
	double y = center.y + radius * std::sin(mid_angle);

	return AcGePoint3d(x, y, center.z);
}

AcGePoint3d BasicTools::GetArcMidPoint(AcDbArc& arc)
{
	AcGePoint3d center=arc.center();
	double radius = arc.radius();
	double startAng = arc.startAngle();
	double endAng = arc.endAngle();
	AcGeVector3d normal = arc.normal();
	AcGeVector3d refVec = AcGeVector3d::kXAxis;
	AcGeCircArc3d arcc(center, normal, refVec, radius, startAng, endAng);
	return GetArcMidPoint(arcc);
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
