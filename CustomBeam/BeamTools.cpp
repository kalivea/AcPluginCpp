#include "stdafx.h"
#include "BeamTools.h"
#include "BeamDwgFields.h"
#include "../Common/DbObjectOwner.h"

bool BeamTools::IsPointInsideBeam(const CPolaCustomBeam* beam, const AcGePoint3d& point, const AcGeTol& tol)
{
	if (!beam || !Pola::BeamFinite(point))
		return false;

	AcGePoint3dArray beam_vertexes = beam->getBeamVertexes();
	double beam_width = beam->getBeamWidth();
	if (!std::isfinite(beam_width) || beam_width <= 0.0) return false;
	int vertex_count = beam->getVertexesNum();

	if (vertex_count < 2 || beam_vertexes.length() < 2)
		return false;

	for (int i = 0; i < vertex_count; i++)
	{
		AcGePoint3d vertex = beam_vertexes[i];
		if (vertex.distanceTo(point) <= beam_width / 2.0)
		{
			return true;
		}
	}

	int segments_count = vertex_count - 1;
	if (beam->IsBeamClosed() && vertex_count >= 3)
		segments_count = vertex_count;

	for (int i = 0; i < segments_count; i++)
	{
		AcGePoint3d start_point = beam_vertexes[i];
		AcGePoint3d end_point;

		if (i == vertex_count - 1 && beam->IsBeamClosed())
			end_point = beam_vertexes[0];
		else
			end_point = beam_vertexes[i + 1];

		if (start_point.isEqualTo(end_point, tol))
			continue;

        // v1 keeps an unused direction at index zero; derive the actual
        // direction from endpoints so old rotated drawings are also safe.
        AcGeVector3d segment_dir = end_point - start_point;
        if (segment_dir.isZeroLength(tol)) continue;
        segment_dir.normalize();

		AcGeVector3d width_direction(-segment_dir.y, segment_dir.x, 0.0);
		if (width_direction.isZeroLength(tol)) continue;
		width_direction.normalize();
		AcGeVector3d segment_vector = BasicTools::GetVectorBetweenTwoPoint(start_point, end_point);
		double segment_length = segment_vector.length();
		AcGeVector3d start_to_point_vector = BasicTools::GetVectorBetweenTwoPoint(start_point, point);
		double projection_distance = start_to_point_vector.dotProduct(segment_dir);


		if (projection_distance >= -tol.equalPoint() && projection_distance <= segment_length + tol.equalPoint())
		{
			AcGePoint3d closest_point = start_point + segment_dir * projection_distance;
			AcGeVector3d closest_to_point_vector = BasicTools::GetVectorBetweenTwoPoint(closest_point, point);
			double width_offset = closest_to_point_vector.dotProduct(width_direction);

			if (fabs(width_offset) <= beam_width / 2.0 + tol.equalPoint())
			{
				return true;
			}
		}

		AcGePoint3d top_left = start_point + width_direction * (beam_width / 2.0);
		AcGePoint3d top_right = end_point + width_direction * (beam_width / 2.0);
		AcGePoint3d bottom_left = start_point - width_direction * (beam_width / 2.0);
		AcGePoint3d bottom_right = end_point - width_direction * (beam_width / 2.0);

		if (IsPointOnLine(point, top_left, top_right, tol) ||
			IsPointOnLine(point, bottom_left, bottom_right, tol) ||
			IsPointOnLine(point, top_left, bottom_left, tol) ||
			IsPointOnLine(point, top_right, bottom_right, tol))
		{
			return true;
		}
	}

	return false;
}

Acad::ErrorStatus BeamTools::GetAllBeamSegmentMidPointChecked(const CPolaCustomBeam* beam, AcGePoint3dArray& points) {
    points.removeAll();
    if (!beam || beam->getVertexesNum() < 2) return Acad::eInvalidInput;
    const AcGePoint3dArray vertices = beam->getBeamVertexes();
    for (int i = 1; i < vertices.length(); ++i)
        points.append(BasicTools::GetMidPoint(vertices[i - 1], vertices[i]));
    return Acad::eOk;
}
Acad::ErrorStatus BeamTools::GetAllPillarInBeamChecked(const CPolaCustomBeam* beam, AcDbObjectIdArray& ids) {
    ids.removeAll();
    if (!beam || beam->getVertexesNum() < 2) return Acad::eInvalidInput;
    AcDbObjectIdArray all, result;
    Acad::ErrorStatus es = SelectEntitys::GetAllEntitysByTypeChecked(CPolaCustomPillar::desc(), all);
    if (es != Acad::eOk) return es;
    for (AcDbObjectId id : all) {
        CPolaCustomPillar* pointer = nullptr;
        if ((es = acdbOpenObject(pointer, id, AcDb::kForRead)) != Acad::eOk) return es;
        Pola::OpenedObject<CPolaCustomPillar> pillar(pointer);
        if (IsPointInsideBeam(beam, pillar->getCenterPoint())) result.append(id);
        if ((es = Pola::Close(pillar)) != Acad::eOk) return es;
    }
    ids = std::move(result);
    return Acad::eOk;
}
Acad::ErrorStatus BeamTools::GetAllPillarCenterInBeamChecked(const CPolaCustomBeam* beam, AcGePoint3dArray& points) {
    points.removeAll();
    AcDbObjectIdArray ids;
    Acad::ErrorStatus es = GetAllPillarInBeamChecked(beam, ids);
    if (es != Acad::eOk) return es;
    AcGePoint3dArray result;
    for (AcDbObjectId id : ids) {
        CPolaCustomPillar* pointer = nullptr;
        if ((es = acdbOpenObject(pointer, id, AcDb::kForRead)) != Acad::eOk) return es;
        Pola::OpenedObject<CPolaCustomPillar> pillar(pointer);
        result.append(pillar->getCenterPoint());
        if ((es = Pola::Close(pillar)) != Acad::eOk) return es;
    }
    points = std::move(result);
    return Acad::eOk;
}
namespace {
void ReportBeamToolsError(Acad::ErrorStatus es) {
    if (es != Acad::eOk) acutPrintf(_T("\nBeam query failed: %d\n"), static_cast<int>(es));
}
}
bool BeamTools::GetAllBeamSegmentMidPoint(const CPolaCustomBeam* beam, AcGePoint3dArray& points) {
    const Acad::ErrorStatus es = GetAllBeamSegmentMidPointChecked(beam, points);
    ReportBeamToolsError(es);
    return es == Acad::eOk && !points.isEmpty();
}
bool BeamTools::GetAllPillarCenterInBeam(const CPolaCustomBeam* beam, AcGePoint3dArray& points) {
    const Acad::ErrorStatus es = GetAllPillarCenterInBeamChecked(beam, points);
    ReportBeamToolsError(es);
    return es == Acad::eOk && !points.isEmpty();
}
bool BeamTools::GetAllPillarInBeam(const CPolaCustomBeam* beam, AcDbObjectIdArray& ids) {
    const Acad::ErrorStatus es = GetAllPillarInBeamChecked(beam, ids);
    ReportBeamToolsError(es);
    return es == Acad::eOk && !ids.isEmpty();
}

bool BeamTools::IsPointOnLine(const AcGePoint3d& point, const AcGePoint3d& line_start, const AcGePoint3d& line_end, const AcGeTol& tol)
{
	AcGeLineSeg3d line(line_start, line_end);
	double param;

	if (line.isOn(point, param, tol))
	{
		return param >= 0.0 - tol.equalPoint() && param <= 1.0 + tol.equalPoint();
	}
	return false;
}
