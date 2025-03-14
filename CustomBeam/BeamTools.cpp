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

        double dx = point.x - vertex.x;
        double dy = point.y - vertex.y;
        double distance_squared = dx * dx + dy * dy;

        if (distance_squared <= (beam_width / 2) * (beam_width / 2))
            return true;
    }

    for (int i = 0; i < vertex_count - 1; i++)
    {
        AcGePoint3d start_point = beam_vertexes[i];
        AcGePoint3d end_point = beam_vertexes[i + 1];

        if (start_point.isEqualTo(end_point, tol))
            continue;

        AcGeVector3d segment_dir;
        if (i == 0 && segment_directions.length() > 0 && segment_directions[0].isZeroLength(tol))
        {
            if (segment_directions.length() > 1)
                segment_dir = segment_directions[1];
            else
                segment_dir = (end_point - start_point).normalize();
        }
        else if (i < segment_directions.length())
        {
            segment_dir = segment_directions[i];
            if (segment_dir.isZeroLength(tol))
                segment_dir = (end_point - start_point).normalize();
        }
        else
        {
            segment_dir = (end_point - start_point).normalize();
        }

        AcGeVector3d width_direction(-segment_dir.y, segment_dir.x, 0.0);
        width_direction.normalize();


        AcGeVector3d segment_vector = end_point - start_point;
        double segment_length = segment_vector.length();
        AcGeVector3d start_to_point_vector = point - start_point;

        double projection_distance = start_to_point_vector.dotProduct(segment_dir);


        if (projection_distance < -tol.equalPoint() || projection_distance > segment_length + tol.equalPoint())
            continue;

        AcGePoint3d closest_point = start_point + segment_dir * projection_distance;

        AcGeVector3d point_to_closest_vector = point - closest_point;
        double width_offset = point_to_closest_vector.dotProduct(width_direction);

        if (fabs(width_offset) <= beam_width / 2 + tol.equalPoint())
        {
            return true;
        }

        AcGePoint3d top_left = start_point + width_direction * (beam_width / 2);
        AcGePoint3d top_right = end_point + width_direction * (beam_width / 2);
        AcGePoint3d bottom_left = start_point - width_direction * (beam_width / 2);
        AcGePoint3d bottom_right = end_point - width_direction * (beam_width / 2);

        if (IsPointOnLine(point, top_left, top_right, tol) ||
            IsPointOnLine(point, bottom_left, bottom_right, tol) ||
            IsPointOnLine(point, top_left, bottom_left, tol) ||
            IsPointOnLine(point, top_right, bottom_right, tol))
        {
            return true;
        }
    }

    if (beam->IsBeamClosed() && vertex_count >= 3)
    {
        AcGePoint3d start_point = beam_vertexes[vertex_count - 1];
        AcGePoint3d end_point = beam_vertexes[0];

        if (!start_point.isEqualTo(end_point, tol))
        {
            AcGeVector3d segment_direction = (end_point - start_point).normalize();
            AcGeVector3d width_direction(-segment_direction.y, segment_direction.x, 0.0);
            width_direction.normalize();

            AcGeVector3d segment_vector = end_point - start_point;
            double segment_length = segment_vector.length();
            AcGeVector3d start_to_point_vector = point - start_point;

            double projection_distance = start_to_point_vector.dotProduct(segment_direction);

            if (projection_distance >= -tol.equalPoint() && projection_distance <= segment_length + tol.equalPoint())
            {
                AcGePoint3d closest_point = start_point + segment_direction * projection_distance;
                AcGeVector3d point_to_closest_vector = point - closest_point;
                double width_offset = point_to_closest_vector.dotProduct(width_direction);

                if (fabs(width_offset) <= beam_width / 2 + tol.equalPoint())
                {
                    return true;
                }

                AcGePoint3d top_left = start_point + width_direction * (beam_width / 2);
                AcGePoint3d top_right = end_point + width_direction * (beam_width / 2);
                AcGePoint3d bottom_left = start_point - width_direction * (beam_width / 2);
                AcGePoint3d bottom_right = end_point - width_direction * (beam_width / 2);

                if (IsPointOnLine(point, top_left, top_right, tol) ||
                    IsPointOnLine(point, bottom_left, bottom_right, tol) ||
                    IsPointOnLine(point, top_left, bottom_left, tol) ||
                    IsPointOnLine(point, top_right, bottom_right, tol))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

inline bool BeamTools::IsPointOnLine(const AcGePoint3d& point, const AcGePoint3d& line_start, const AcGePoint3d& line_end, const AcGeTol& tol)
{

    AcGeVector3d v1 = point - line_start;

    AcGeVector3d v2 = line_end - line_start;

    double v2_length = v2.length();
    if (v2_length < tol.equalPoint())
        return false;

    v2 = v2 / v2_length;
  
    double projection = v1.dotProduct(v2);

    if (projection < -tol.equalPoint() || projection > v2_length + tol.equalPoint())
        return false;

    AcGePoint3d closest_point_on_line = line_start + v2 * projection;

    return point.isEqualTo(closest_point_on_line, tol);
}
