#include "StdAfx.h"
#include "DrawEntity.h"
#include "DbObjectOwner.h"
#include "dbmleader.h"
#include "dbmleaderstyle.h"
#include <cmath>
#include <cstring>

namespace
{
constexpr double pi = 3.14159265358979323846;
bool finitePoint(const AcGePoint3d& p) { return std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z); }
bool finitePoint(const AcGePoint2d& p) { return std::isfinite(p.x) && std::isfinite(p.y); }
Acad::ErrorStatus angleInRadians(double angle, int type, double& radians)
{
    if (!std::isfinite(angle) || (type != 0 && type != 1)) return Acad::eInvalidInput;
    radians = type == 0 ? BasicTools::ConvertAngle(angle, 1) : angle;
    return std::isfinite(radians) ? Acad::eOk : Acad::eInvalidInput;
}
template<class T> Acad::ErrorStatus post(Pola::DbObjectOwner<T>& entity, AcDbObjectId& id)
{
    Acad::ErrorStatus status = AddToModelSpace::AddEntityToModelSpace(entity.get(), id, false);
    if (!entity->objectId().isNull())
    {
        const Acad::ErrorStatus cleanup = entity.release()->close();
        if (status == Acad::eOk) status = cleanup;
    }
    if (status != Acad::eOk) id = AcDbObjectId::kNull;
    return status;
}
void report(Acad::ErrorStatus status)
{
    if (status != Acad::eOk) acutPrintf(_T("\nDrawing entity failed: %d"), static_cast<int>(status));
}
}

Acad::ErrorStatus DrawEntity::DrawLine(const AcGePoint3d& p1, const AcGePoint3d& p2, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(p1) || !finitePoint(p2) || p1.isEqualTo(p2)) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbLine> line(new AcDbLine(p1, p2));
    return post(line, id);
}
Acad::ErrorStatus DrawEntity::DrawLine(const AcGePoint3d& point, const double& length, const double& angle, const int& type, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    double radians = 0;
    Acad::ErrorStatus status = angleInRadians(angle, type, radians);
    if (status != Acad::eOk || !finitePoint(point) || !std::isfinite(length) || length <= 0) return Acad::eInvalidInput;
    return DrawLine(point, AcGePoint3d(point.x + length * cos(radians), point.y + length * sin(radians), point.z), id);
}
Acad::ErrorStatus DrawEntity::DrawCircle(const AcGePoint3d& center, const double& radius, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(center) || !std::isfinite(radius) || radius <= 0) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbCircle> circle(new AcDbCircle());
    Acad::ErrorStatus status = circle->setCenter(center);
    if (status == Acad::eOk) status = circle->setRadius(radius);
    return status == Acad::eOk ? post(circle, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawCircle(const AcGePoint3d& p1, const AcGePoint3d& p2, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(p1) || !finitePoint(p2)) return Acad::eInvalidInput;
    return DrawCircle(BasicTools::GetMidPoint(p1, p2), p1.distanceTo(p2) / 2.0, id);
}
Acad::ErrorStatus DrawEntity::DrawCircle(const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p3, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(p1) || !finitePoint(p2) || !finitePoint(p3) || BasicTools::IsCollinearPoint(p1, p2, p3)) return Acad::eInvalidInput;
    const AcGeCircArc3d geometry(p1, p2, p3);
    Pola::DbObjectOwner<AcDbCircle> circle(new AcDbCircle());
    Acad::ErrorStatus status = circle->setCenter(geometry.center());
    if (status == Acad::eOk) status = circle->setNormal(geometry.normal());
    if (status == Acad::eOk) status = circle->setRadius(geometry.radius());
    return status == Acad::eOk ? post(circle, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawArc(const AcGePoint3d& p1, const AcGePoint3d& p2, const AcGePoint3d& p3, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(p1) || !finitePoint(p2) || !finitePoint(p3) || BasicTools::IsCollinearPoint(p1, p2, p3)) return Acad::eInvalidInput;
    const AcGeCircArc3d geometry(p1, p2, p3);
    Pola::DbObjectOwner<AcDbArc> arc(new AcDbArc());
    Acad::ErrorStatus status = arc->setFromAcGeCurve(geometry);
    return status == Acad::eOk ? post(arc, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawArc(const AcGePoint3d& center, const AcGePoint3d& start, const double& angle, const int type, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    double radians = 0;
    Acad::ErrorStatus status = angleInRadians(angle, type, radians);
    if (status != Acad::eOk || !finitePoint(center) || !finitePoint(start) || center.isEqualTo(start) || radians <= 0 || radians > 2 * pi) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbArc> arc(new AcDbArc());
    status = arc->setCenter(center);
    if (status == Acad::eOk) status = arc->setRadius(center.distanceTo(start));
    const double start_angle = BasicTools::GetAngleToXaxis(start - center, 1);
    if (status == Acad::eOk) status = arc->setStartAngle(start_angle);
    if (status == Acad::eOk) status = arc->setEndAngle(start_angle + radians);
    return status == Acad::eOk ? post(arc, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawArc(const AcGePoint3d& center, const AcGePoint3d& start, const AcGePoint3d& end, int n, AcDbObjectId& id)
{
    (void)n; // retained legacy disambiguation argument
    id = AcDbObjectId::kNull;
    if (!finitePoint(center) || !finitePoint(start) || !finitePoint(end) || center.isEqualTo(start) || start.isEqualTo(end)) return Acad::eInvalidInput;
    const double radius = center.distanceTo(start);
    if (std::abs(radius - center.distanceTo(end)) > AcGeContext::gTol.equalPoint()) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbArc> arc(new AcDbArc());
    Acad::ErrorStatus status = arc->setCenter(center);
    if (status == Acad::eOk) status = arc->setRadius(radius);
    if (status == Acad::eOk) status = arc->setStartAngle(BasicTools::GetAngleToXaxis(start - center, 1));
    if (status == Acad::eOk) status = arc->setEndAngle(BasicTools::GetAngleToXaxis(end - center, 1));
    return status == Acad::eOk ? post(arc, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawPolyLine(const AcGePoint2dArray& points, const double& width, bool closed, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (points.length() < 2 || !std::isfinite(width) || width < 0) return Acad::eInvalidInput;
    for (int i = 0; i < points.length(); ++i) if (!finitePoint(points.at(i))) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbPolyline> polyline(new AcDbPolyline());
    for (int i = 0; i < points.length(); ++i)
    {
        const Acad::ErrorStatus status = polyline->addVertexAt(i, points.at(i));
        if (status != Acad::eOk) return status;
    }
    polyline->setClosed(closed);
    const Acad::ErrorStatus status = polyline->setConstantWidth(width);
    return status == Acad::eOk ? post(polyline, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawRectangle(const AcGePoint3d& p1, const AcGePoint3d& p2, const double& width, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(p1) || !finitePoint(p2) || !BasicTools::CanDrawRect(p1, p2)) return Acad::eInvalidInput;
    AcGePoint2dArray points;
    points.append(AcGePoint2d(BasicTools::Min(p1.x, p2.x), BasicTools::Min(p1.y, p2.y)));
    points.append(AcGePoint2d(BasicTools::Max(p1.x, p2.x), BasicTools::Min(p1.y, p2.y)));
    points.append(AcGePoint2d(BasicTools::Max(p1.x, p2.x), BasicTools::Max(p1.y, p2.y)));
    points.append(AcGePoint2d(BasicTools::Min(p1.x, p2.x), BasicTools::Max(p1.y, p2.y)));
    return DrawPolyLine(points, width, true, id);
}
Acad::ErrorStatus DrawEntity::HatchPattern(const TCHAR* name, const double& scale, const double& angle, const int& type, bool associative, const AcDbObjectIdArray& boundaries, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    double radians = 0;
    Acad::ErrorStatus status = angleInRadians(angle, type, radians);
    if (status != Acad::eOk || name == nullptr || *name == 0 || !std::isfinite(scale) || scale <= 0 || boundaries.isEmpty()) return Acad::eInvalidInput;
    std::vector<Pola::OpenedObject<AcDbEntity>> boundary_objects;
    for (int i = 0; i < boundaries.length(); ++i)
    {
        if (boundaries.at(i).isNull()) return Acad::eInvalidInput;
        AcDbEntity* pointer = nullptr;
        status = acdbOpenObject(pointer, boundaries.at(i), associative ? AcDb::kForWrite : AcDb::kForRead);
        if (status != Acad::eOk) return status;
        boundary_objects.emplace_back(pointer);
    }
    Pola::DbObjectOwner<AcDbHatch> hatch(new AcDbHatch());
    status = hatch->setNormal(AcGeVector3d::kZAxis);
    if (status == Acad::eOk) status = hatch->setElevation(0);
    if (status == Acad::eOk) status = hatch->setAssociative(associative);
    if (status == Acad::eOk) status = hatch->setPatternAngle(radians);
    if (status == Acad::eOk) status = hatch->setPatternScale(scale);
    if (status == Acad::eOk) status = hatch->setPattern(AcDbHatch::kPreDefined, name);
    if (status == Acad::eOk) status = hatch->appendLoop(AcDbHatch::kExternal, boundaries);
    if (status == Acad::eOk) status = hatch->evaluateHatch();
    AcDbObjectId inserted;
    if (status == Acad::eOk) status = AddToModelSpace::AddEntityToModelSpace(hatch.get(), inserted, false);
    size_t reactors_added = 0;
    if (status == Acad::eOk && associative)
    {
        for (; reactors_added < boundary_objects.size(); ++reactors_added)
        {
            status = boundary_objects[reactors_added]->addPersistentReactor(inserted);
            if (status != Acad::eOk) break;
        }
    }
    if (status != Acad::eOk && !hatch->objectId().isNull())
    {
        while (reactors_added > 0)
        {
            const Acad::ErrorStatus cleanup = boundary_objects[--reactors_added]->removePersistentReactor(hatch->objectId());
            if (cleanup != Acad::eOk) report(cleanup);
        }
        const Acad::ErrorStatus cleanup = hatch->erase();
        if (cleanup != Acad::eOk) report(cleanup);
    }
    for (auto& boundary : boundary_objects) status = Pola::Close(boundary, status);
    if (!hatch->objectId().isNull())
    {
        const Acad::ErrorStatus cleanup = hatch.release()->close();
        if (status == Acad::eOk) status = cleanup;
    }
    if (status == Acad::eOk) id = inserted;
    return status;
}
Acad::ErrorStatus DrawEntity::AddText(const AcGePoint3d& point, const TCHAR* content, const AcDbObjectId style, const double& height, const double& rotation, const double& factor, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(point) || content == nullptr || !std::isfinite(height) || height <= 0 || !std::isfinite(rotation) || !std::isfinite(factor) || factor <= 0) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbText> text(new AcDbText());
    text->setDatabaseDefaults();
    Acad::ErrorStatus status = text->setPosition(point);
    if (status == Acad::eOk) status = text->setTextString(content);
    if (status == Acad::eOk && !style.isNull()) status = text->setTextStyle(style);
    if (status == Acad::eOk) status = text->setHeight(height);
    if (status == Acad::eOk) status = text->setRotation(rotation);
    if (status == Acad::eOk) status = text->setWidthFactor(factor);
    return status == Acad::eOk ? post(text, id) : status;
}
Acad::ErrorStatus DrawEntity::AlignedDimension(const AcGePoint3d& start, const AcGePoint3d& end, const AcGePoint3d& line, const AcDbObjectId style, const TCHAR* content, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(start) || !finitePoint(end) || !finitePoint(line) || start.isEqualTo(end)) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbAlignedDimension> dimension(new AcDbAlignedDimension());
    dimension->setDatabaseDefaults();
    Acad::ErrorStatus status = dimension->setXLine1Point(start);
    if (status == Acad::eOk) status = dimension->setXLine2Point(end);
    if (status == Acad::eOk) status = dimension->setDimLinePoint(line);
    if (status == Acad::eOk && content != nullptr) status = dimension->setDimensionText(content);
    if (status == Acad::eOk && !style.isNull()) status = dimension->setDimstyleData(style);
    return status == Acad::eOk ? post(dimension, id) : status;
}
Acad::ErrorStatus DrawEntity::DrawFrame(char* size, const AcGePoint3d& point, const double& scale)
{
    if (size == nullptr || !finitePoint(point) || !std::isfinite(scale) || scale <= 0) return Acad::eInvalidInput;
    double width = 0;
    if (strcmp(size, "A2") == 0) width = 594;
    else if (strcmp(size, "A2+1/4") == 0) width = 743;
    else if (strcmp(size, "A2+1/2") == 0) width = 891;
    else if (strcmp(size, "A2+3/4") == 0) width = 1040;
    else return Acad::eInvalidInput;
    AcDbObjectId outside;
    Acad::ErrorStatus status = DrawRectangle(point, AcGePoint3d(point.x + width * scale, point.y + 420 * scale, point.z), 0, outside);
    if (status != Acad::eOk) return status;
    AcDbObjectId inside;
    status = DrawRectangle(AcGePoint3d(point.x + 25 * scale, point.y + 10 * scale, point.z), AcGePoint3d(point.x + (width - 10) * scale, point.y + 410 * scale, point.z), 0.35 * scale, inside);
    if (status != Acad::eOk)
    {
        const Acad::ErrorStatus cleanup = EditEntity::DeleteEntity(outside);
        if (cleanup != Acad::eOk) report(cleanup);
    }
    return status;
}
Acad::ErrorStatus DrawEntity::AddMLeader(const AcGePoint3d& point, const AcGePoint3d& leader_point, const AcGePoint3d& text_point, const TCHAR* content, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!finitePoint(point) || !finitePoint(leader_point) || !finitePoint(text_point) || content == nullptr) return Acad::eInvalidInput;
    AcDbObjectId text_style;
    Acad::ErrorStatus status = StyleTools::CreateTextStyle(_T("default_txt"), _T("gbcbig.shx"), _T("gbenor.shx"), 0.75, text_style);
    if (status != Acad::eOk) return status;
    AcDbObjectId leader_style;
    status = StyleTools::GetMLeaderStyleId(_T("NONE_ARROW"), leader_style);
    if (status == Acad::eKeyNotFound)
    {
        Pola::DbObjectOwner<AcDbMLeaderStyle> style(new AcDbMLeaderStyle());
        status = style->setName(_T("NONE_ARROW"));
        if (status == Acad::eOk) status = style->setArrowSymbolId(_T("_NONE"));
        if (status == Acad::eOk) status = style->postMLeaderStyleToDb(acdbHostApplicationServices()->workingDatabase(), _T("NONE_ARROW"), leader_style);
        if (status == Acad::eOk) status = style.release()->close();
    }
    if (status != Acad::eOk) return status;
    Pola::DbObjectOwner<AcDbMText> text(new AcDbMText());
    text->setContents(content); // returns content length, not ErrorStatus
    status = text->setAttachment(AcDbMText::kMiddleCenter);
    if (status == Acad::eOk) status = text->setLocation(text_point);
    if (status == Acad::eOk) status = text->setTextHeight(350);
    if (status == Acad::eOk) status = text->setTextStyle(text_style);
    Pola::DbObjectOwner<AcDbMLeader> leader(new AcDbMLeader());
    int index = -1;
    if (status == Acad::eOk) status = leader->setMLeaderStyle(leader_style);
    if (status == Acad::eOk) status = leader->addLeaderLine(point, index);
    if (status == Acad::eOk) status = leader->addFirstVertex(index, point);
    if (status == Acad::eOk) status = leader->setTextAttachmentType(AcDbMLeaderStyle::kAttachmentBottomOfTopLine);
    if (status == Acad::eOk) status = leader->setLastVertex(index, leader_point);
    if (status == Acad::eOk) status = leader->setMText(text.get()); // SDK copies input; text stays transient
    if (status == Acad::eOk) status = leader->setContentType(AcDbMLeaderStyle::kMTextContent);
    if (status == Acad::eOk) status = leader->setDoglegDirection(index, AcGeVector3d::kXAxis);
    if (status == Acad::eOk) status = leader->setEnableDogleg(true);
    return status == Acad::eOk ? post(leader, id) : status;
}

// The legacy ID wrappers retain source compatibility and report checked errors.
AcDbObjectId DrawEntity::DrawLine(const AcGePoint3d& a, const AcGePoint3d& b) { AcDbObjectId id; report(DrawLine(a,b,id)); return id; }
AcDbObjectId DrawEntity::DrawLine(const AcGePoint3d& p, const double& l, const double& a, const int& t) { AcDbObjectId id; report(DrawLine(p,l,a,t,id)); return id; }
AcDbObjectId DrawEntity::DrawCircle(const AcGePoint3d& p, const double& r) { AcDbObjectId id; report(DrawCircle(p,r,id)); return id; }
AcDbObjectId DrawEntity::DrawCircle(const AcGePoint3d& a, const AcGePoint3d& b) { AcDbObjectId id; report(DrawCircle(a,b,id)); return id; }
AcDbObjectId DrawEntity::DrawCircle(const AcGePoint3d& a, const AcGePoint3d& b, const AcGePoint3d& c) { AcDbObjectId id; report(DrawCircle(a,b,c,id)); return id; }
AcDbObjectId DrawEntity::DrawArc(const AcGePoint3d& a, const AcGePoint3d& b, const AcGePoint3d& c) { AcDbObjectId id; report(DrawArc(a,b,c,id)); return id; }
AcDbObjectId DrawEntity::DrawArc(const AcGePoint3d& c, const AcGePoint3d& s, const double& a, const int t) { AcDbObjectId id; report(DrawArc(c,s,a,t,id)); return id; }
AcDbObjectId DrawEntity::DrawArc(const AcGePoint3d& c, const AcGePoint3d& s, const AcGePoint3d& e, int n) { AcDbObjectId id; report(DrawArc(c,s,e,n,id)); return id; }
AcDbObjectId DrawEntity::DrawPolyLine(const AcGePoint2dArray& p, const double& w, bool c) { AcDbObjectId id; report(DrawPolyLine(p,w,c,id)); return id; }
AcDbObjectId DrawEntity::DrawRectangle(const AcGePoint3d& a, const AcGePoint3d& b, const double& w) { AcDbObjectId id; report(DrawRectangle(a,b,w,id)); return id; }
AcDbObjectId DrawEntity::HatchPattern(const TCHAR* n, const double& s, const double& a, const int& t, bool associative, const AcDbObjectIdArray& boundaries) { AcDbObjectId id; report(HatchPattern(n,s,a,t,associative,boundaries,id)); return id; }
AcDbObjectId DrawEntity::AddText(const AcGePoint3d& p, const TCHAR* c, const AcDbObjectId s, const double& h, const double& r, const double& w) { AcDbObjectId id; report(AddText(p,c,s,h,r,w,id)); return id; }
AcDbObjectId DrawEntity::AlignedDimension(const AcGePoint3d& a, const AcGePoint3d& b, const AcGePoint3d& c, const AcDbObjectId s, const TCHAR* t) { AcDbObjectId id; report(AlignedDimension(a,b,c,s,t,id)); return id; }
AcDbObjectId DrawEntity::AddMLeader(const AcGePoint3d& p, const AcGePoint3d& l, const AcGePoint3d& t, const TCHAR* c) { AcDbObjectId id; report(AddMLeader(p,l,t,c,id)); return id; }
AcDbObjectId DrawEntity::AddMtext() { report(Acad::eNotImplementedYet); return AcDbObjectId::kNull; }
AcDbObjectId DrawEntity::RotatedDimension() { report(Acad::eNotImplementedYet); return AcDbObjectId::kNull; }
