#include "stdafx.h"
#include "BeamTools.h"

bool BeamTools::IsPointInsideBeam(const CPolaCustomBeam* beam, const AcGePoint3d& point, const AcGeTol& tol)
{
	if (!beam)
		return false;

	AcGePoint3dArray beam_vertexes = beam->getBeamVertexes();
	double beam_width = beam->getBeamWidth();
	AcGeVector3dArray segment_directions = beam->getBeamSegmentDirection();
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

		AcGeVector3d segment_dir;

		if (i < segment_directions.length())
		{
			if (segment_directions[i].isZeroLength(tol))
			{
				segment_dir = BasicTools::GetVectorBetweenTwoPoint(start_point, end_point).normalize();
			}
			else
			{
				segment_dir = segment_directions[i].normalize();
			}
		}
		else
		{
			segment_dir = BasicTools::GetVectorBetweenTwoPoint(start_point, end_point).normalize();
		}

		AcGeVector3d width_direction(-segment_dir.y, segment_dir.x, 0.0);
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