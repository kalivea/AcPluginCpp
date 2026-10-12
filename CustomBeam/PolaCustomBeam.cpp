// (C) Copyright 2002-2007 by Autodesk, Inc.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- PolaCustomBeam.cpp : Implementation of CPolaCustomBeam
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PolaCustomBeam.h"
#include "aced.h"

//
#include "PillarTools.h"
#include "BeamDwgFields.h"
#include "../Common/DbObjectOwner.h"
#include <memory>
#include <cmath>
//-----------------------------------------------------------------------------
Adesk::UInt32 CPolaCustomBeam::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	CPolaCustomBeam, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, POLACUSTOMBEAM,
	POLACUSTOMBEAMAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
CPolaCustomBeam::CPolaCustomBeam() : AcDbEntity() {
}

CPolaCustomBeam::~CPolaCustomBeam() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CPolaCustomBeam::dwgOutFields(AcDbDwgFiler* pFiler) const {
    assertReadEnabled();
    if (!pFiler) return Acad::eInvalidInput;
    Pola::BeamDwgFields fields;
    fields.count = vertexes_num_;
    fields.vertices = beam_vertexes_;
    fields.top = top_offset_vertex_;
    fields.bottom = bottom_offset_vertex_;
    fields.width = beam_b_;
    fields.height = beam_h_;
    fields.visible = beam_viewable_;
    fields.property = beam_property_;
    fields.directions = beam_segment_direction_;
    Acad::ErrorStatus es = Pola::ValidateBeamFields(fields);
    if (es != Acad::eOk) return es;
    if ((es = AcDbEntity::dwgOutFields(pFiler)) != Acad::eOk) return es;
    return Pola::WriteBeamFields(pFiler, fields);
}

Acad::ErrorStatus CPolaCustomBeam::dwgInFields(AcDbDwgFiler* pFiler) {
    assertWriteEnabled();
    if (!pFiler) return Acad::eInvalidInput;
    Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
    if (es != Acad::eOk) return es;
    Pola::BeamDwgFields fields;
    if ((es = Pola::ReadBeamFields(pFiler, fields)) != Acad::eOk) return es;
    beam_vertexes_ = std::move(fields.vertices);
    top_offset_vertex_ = std::move(fields.top);
    bottom_offset_vertex_ = std::move(fields.bottom);
    beam_segment_direction_ = std::move(fields.directions);
    beam_viewable_ = std::move(fields.visible);
    vertexes_num_ = fields.count;
    beam_b_ = fields.width;
    beam_h_ = fields.height;
    beam_property_ = fields.property;
    return Acad::eOk;
}

bool CPolaCustomBeam::HasDrawableGeometry() const {
    return vertexes_num_ >= 2 && vertexes_num_ <= Pola::kMaxBeamVertices &&
        beam_vertexes_.length() == vertexes_num_ &&
        top_offset_vertex_.length() == vertexes_num_ &&
        bottom_offset_vertex_.length() == vertexes_num_ &&
        beam_segment_direction_.length() == vertexes_num_ &&
        beam_viewable_.size() == static_cast<size_t>(vertexes_num_);
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaCustomBeam::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	if (!mode || !HasDrawableGeometry())
	{
		return mode ? AcDbEntity::subWorldDraw(mode) : Adesk::kFalse;
	}
	for (int i = 0;i < vertexes_num_ - 1;i++)
	{
		AcGePoint3d center_line_segment_start = beam_vertexes_.at(i);
		AcGePoint3d center_line_segment_end = beam_vertexes_.at(i + 1);
		AcDbLine temp_center_line(center_line_segment_start, center_line_segment_end);

		AcGePoint3d top_line_segment_start = top_offset_vertex_.at(i);
		AcGePoint3d top_line_segment_end = top_offset_vertex_.at(i + 1);
		AcDbLine temp_top_line(top_line_segment_start, top_line_segment_end);

		AcGePoint3d bottom_line_segment_start = bottom_offset_vertex_.at(i);
		AcGePoint3d bottom_line_segment_end = bottom_offset_vertex_.at(i + 1);
		AcDbLine temp_bottom_line(bottom_line_segment_start, bottom_line_segment_end);

		Adesk::GsMarker base_marker = i * 3;

		if (beam_viewable_.at(i + 1) == 0)
		{
			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
			mode->subEntityTraits().setLineTypeScale(700);
			mode->subEntityTraits().setColor(256);
			mode->subEntityTraits().setSelectionMarker(base_marker);
			temp_top_line.worldDraw(mode);
			mode->subEntityTraits().setSelectionMarker(base_marker + 2);
			temp_bottom_line.worldDraw(mode);

			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CENTER")));
			mode->subEntityTraits().setColor(8);
			mode->subEntityTraits().setSelectionMarker(base_marker + 1);
			temp_center_line.worldDraw(mode);
		}
		else if (beam_viewable_.at(i + 1) == 1)
		{
			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
			mode->subEntityTraits().setSelectionMarker(base_marker);
			mode->subEntityTraits().setColor(256);
			temp_top_line.worldDraw(mode);
			mode->subEntityTraits().setSelectionMarker(base_marker + 2);
			temp_bottom_line.worldDraw(mode);

			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CENTER")));
			mode->subEntityTraits().setLineTypeScale(700);
			mode->subEntityTraits().setColor(8);
			mode->subEntityTraits().setSelectionMarker(base_marker + 1);
			temp_center_line.worldDraw(mode);
		}
		else
		{
			return Adesk::kFalse;
		}
	}
	const bool isClosed = IsBeamClosed();
	if (!isClosed)
	{
		mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
		AcDbLine left_side_line(top_offset_vertex_.first(), bottom_offset_vertex_.first());
		AcDbLine right_side_line(top_offset_vertex_.last(), bottom_offset_vertex_.last());
		mode->subEntityTraits().setSelectionMarker(3 * (vertexes_num_ - 1));
		left_side_line.worldDraw(mode);
		mode->subEntityTraits().setSelectionMarker(3 * (vertexes_num_ - 1) + 1);
		right_side_line.worldDraw(mode);
	}

	return (AcDbEntity::subWorldDraw(mode));
}

Adesk::UInt32 CPolaCustomBeam::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

//- Osnap points protocol
Acad::ErrorStatus CPolaCustomBeam::GetSnapLine(Adesk::GsMarker marker,
    AcGePoint3d& start, AcGePoint3d& end) const {
    if (!HasDrawableGeometry() || marker < 0) return Acad::eInvalidInput;
    const Adesk::GsMarker endMarker = 3 * (vertexes_num_ - 1);
    if (marker >= endMarker) {
        if (IsBeamClosed() || marker > endMarker + 1) return Acad::eInvalidInput;
        const int index = marker == endMarker ? 0 : vertexes_num_ - 1;
        start = top_offset_vertex_[index];
        end = bottom_offset_vertex_[index];
        return Acad::eOk;
    }
    const int index = static_cast<int>(marker / 3);
    const AcGePoint3dArray& vertices = marker % 3 == 0 ? top_offset_vertex_ :
        marker % 3 == 1 ? beam_vertexes_ : bottom_offset_vertex_;
    start = vertices[index];
    end = vertices[index + 1];
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subGetOsnapPoints(
    AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d& pickPoint, const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform, AcGePoint3dArray& snapPoints, AcDbIntArray& geomIds) const {
    assertReadEnabled();
    if (gsSelectionMark == -1)
        return AcDbEntity::subGetOsnapPoints(osnapMode, gsSelectionMark, pickPoint,
            lastPoint, viewXform, snapPoints, geomIds);
    AcGePoint3d start, end;
    Acad::ErrorStatus es = GetSnapLine(gsSelectionMark, start, end);
    if (es != Acad::eOk) return es;
    AcDbLine line(start, end);
    return line.getOsnapPoints(osnapMode, -1, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
}

Acad::ErrorStatus CPolaCustomBeam::subGetOsnapPoints(
    AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d& pickPoint, const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform, AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds, const AcGeMatrix3d& insertionMat) const {
    assertReadEnabled();
    if (gsSelectionMark == -1)
        return AcDbEntity::subGetOsnapPoints(osnapMode, gsSelectionMark, pickPoint,
            lastPoint, viewXform, snapPoints, geomIds, insertionMat);
    AcGePoint3d start, end;
    Acad::ErrorStatus es = GetSnapLine(gsSelectionMark, start, end);
    if (es != Acad::eOk) return es;
    AcDbLine line(start, end);
    return line.getOsnapPoints(osnapMode, -1, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
}

//- Grip points protocol
Acad::ErrorStatus CPolaCustomBeam::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	//----- This method is never called unless you return eNotImplemented
	//----- from the new getGripPoints() method below (which is the default implementation)

	//return (AcDbEntity::subGetGripPoints(gripPoints, osnapModes, geomIds));
	//for (int i = 0;i < vertexes_num_;i++)
	//{
	//	gripPoints.append(beam_vertexes_.at(i));
	//}
	for (const auto& vertex : beam_vertexes_)
	{
		gripPoints.append(vertex);
	}
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset) {
    assertWriteEnabled();
    if (!HasDrawableGeometry() || !Pola::BeamFinite(offset)) return Acad::eInvalidInput;
    AcGePoint3dArray vertices = beam_vertexes_;
    for (int index : indices) {
        if (index < 0 || index >= vertices.length()) return Acad::eInvalidInput;
        vertices[index] += offset;
    }
    Acad::ErrorStatus es = setBeamVertexes(vertices);
    if (es != Acad::eOk) return es;
    recordGraphicsModified(true);
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subGetGripPoints(
	AcDbGripDataPtrArray & grips, const double curViewUnitSize, const int gripSize,
	const AcGeVector3d & curViewDir, const int bitflags
) const {
	assertReadEnabled();

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.

	return (AcDbEntity::subGetGripPoints(grips, curViewUnitSize, gripSize, curViewDir, bitflags));

}

Acad::ErrorStatus CPolaCustomBeam::subMoveGripPointsAt(
	const AcDbVoidPtrArray & gripAppData, const AcGeVector3d & offset,
	const int bitflags
) {
	assertWriteEnabled();

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt(gripAppData, offset, bitflags));
}

AcGePoint3dArray CPolaCustomBeam::getBeamVertexes() const
{
	assertReadEnabled();
	return beam_vertexes_;
}

double CPolaCustomBeam::getBeamWidth() const
{
	assertReadEnabled();
	return beam_b_;
}

double CPolaCustomBeam::getBeamHeight() const
{
	assertReadEnabled();
	return beam_h_;
}

std::vector<Adesk::Int32> CPolaCustomBeam::getBeamViewable() const
{
	assertReadEnabled();
	return beam_viewable_;
}

Adesk::Int32 CPolaCustomBeam::getViewableAt(const int index) const
{
	assertReadEnabled();
	return index >= 0 && index < static_cast<int>(beam_viewable_.size()) ? beam_viewable_[index] : -1;
}

Adesk::Int32 CPolaCustomBeam::getBeamProperty() const
{
	assertReadEnabled();
	return beam_property_;
}

Adesk::Int32 CPolaCustomBeam::getVertexesNum() const
{
	assertReadEnabled();
	return beam_vertexes_.length();
}

double CPolaCustomBeam::getBeamLength() const
{
	assertReadEnabled();
	double length = 0.0;
	for (int i = 0; i < vertexes_num_ - 1; i++)
	{
		length += beam_vertexes_[i].distanceTo(beam_vertexes_[i + 1]);
	}

	if (IsBeamClosed() && vertexes_num_ > 2)
	{
		length += beam_vertexes_[vertexes_num_ - 1].distanceTo(beam_vertexes_[0]);
	}
	return length;
}

Adesk::Int32 CPolaCustomBeam::getBeamSegmentNum() const
{
	return beam_vertexes_.length() > 1 ? beam_vertexes_.length() - 1 : 0;
}

bool CPolaCustomBeam::IsBeamClosed() const
{
	return beam_vertexes_.length() >= 3 && beam_vertexes_.first().isEqualTo(beam_vertexes_.last());
}

AcGeVector3dArray CPolaCustomBeam::getBeamSegmentDirection() const
{
	assertReadEnabled();
	return beam_segment_direction_;
}

AcGePoint3d CPolaCustomBeam::getHorizontalMidPoint() const
{
	if (vertexes_num_ <= 0)
		return AcGePoint3d(6496, 6496, 6496);

	if (vertexes_num_ == 1)
		return beam_vertexes_[0];

	double total_length = getBeamLength();

	double half_length = total_length / 2.0;
	double current_length = 0.0;

	for (int i = 0; i < vertexes_num_ - 1; i++)
	{
		double segment_length = beam_vertexes_[i].distanceTo(beam_vertexes_[i + 1]);

		if (segment_length > 0.0 && current_length + segment_length >= half_length)
		{
			double ratio = (half_length - current_length) / segment_length;
			AcGeVector3d direction = beam_vertexes_[i + 1] - beam_vertexes_[i];
			return beam_vertexes_[i] + direction * ratio;
		}

		current_length += segment_length;
	}
	if (IsBeamClosed() && vertexes_num_ > 2)
	{
		double segment_length = beam_vertexes_[vertexes_num_ - 1].distanceTo(beam_vertexes_[0]);

		if (segment_length > 0.0 && current_length + segment_length >= half_length)
		{
			double ratio = (half_length - current_length) / segment_length;
			AcGeVector3d direction = beam_vertexes_[0] - beam_vertexes_[vertexes_num_ - 1];
			return beam_vertexes_[vertexes_num_ - 1] + direction * ratio;
		}
	}
	return beam_vertexes_[0];
}

AcGePoint3d CPolaCustomBeam::getFirstSegmentMidPoint() const
{
	return beam_vertexes_.length() >= 2 ? BasicTools::GetMidPoint(beam_vertexes_[0], beam_vertexes_[1]) : AcGePoint3d::kOrigin;
}

namespace {
Acad::ErrorStatus BuildBeamGeometry(const AcGePoint3dArray& vertices, double width,
    AcGePoint3dArray& top, AcGePoint3dArray& bottom, AcGeVector3dArray& directions) {
    if (vertices.length() < 2 || vertices.length() > Pola::kMaxBeamVertices ||
        !std::isfinite(width) || width <= 0.0) return Acad::eInvalidInput;
    for (int i = 0; i < vertices.length(); ++i) {
        if (!Pola::BeamFinite(vertices[i])) return Acad::eInvalidInput;
        if (i > 0 && (vertices[i].isEqualTo(vertices[i - 1]) ||
            !Pola::BeamFinite(vertices[i] - vertices[i - 1]))) return Acad::eInvalidInput;
    }
    Acad::ErrorStatus es = BasicTools::OffsetPolyLineChecked(vertices, width * 0.5, top);
    if (es != Acad::eOk) return es;
    if ((es = BasicTools::OffsetPolyLineChecked(vertices, -width * 0.5, bottom)) != Acad::eOk) return es;
    if (top.length() != vertices.length() || bottom.length() != vertices.length())
        return Acad::eInvalidInput;
    directions.append(AcGeVector3d::kIdentity);
    for (int i = 1; i < vertices.length(); ++i) {
        if (!Pola::BeamFinite(top[i]) || !Pola::BeamFinite(bottom[i])) return Acad::eInvalidInput;
        directions.append(vertices[i] - vertices[i - 1]);
    }
    if (!Pola::BeamFinite(top[0]) || !Pola::BeamFinite(bottom[0])) return Acad::eInvalidInput;
    return Acad::eOk;
}
void ReportBeamError(Acad::ErrorStatus es) {
    if (es != Acad::eOk && es != Acad::eUserBreak)
        acutPrintf(_T("\nBeam operation failed: %d\n"), static_cast<int>(es));
}
Acad::ErrorStatus RollbackBeamCreation(CPolaCustomBeam* beam, AcDbObjectId& id, Acad::ErrorStatus error) {
    id = AcDbObjectId::kNull;
    if (beam && !beam->objectId().isNull()) {
        const Acad::ErrorStatus cleanup = beam->erase();
        if (cleanup != Acad::eOk)
            acutPrintf(_T("\nBeam rollback failed: %d\n"), static_cast<int>(cleanup));
    }
    return error;
}
Acad::ErrorStatus PickBeamVisibility(Adesk::Int32& visibility) {
    TCHAR keyword[256] = {};
    if (!InputValue::GetKeyword(_T("Beam segment visibility: [Visible/Invisible]"),
        _T("Visible Invisible"), keyword, sizeof(keyword) / sizeof(TCHAR)))
        return Acad::eUserBreak;
    visibility = _tcscmp(keyword, _T("Invisible")) == 0 ? 0 : 1;
    return Acad::eOk;
}
}

Acad::ErrorStatus CPolaCustomBeam::setBeamVertexes(const AcGePoint3dArray& vertices) {
    assertWriteEnabled();
    AcGePoint3dArray top, bottom;
    AcGeVector3dArray directions;
    Acad::ErrorStatus es = BuildBeamGeometry(vertices, beam_b_, top, bottom, directions);
    if (es != Acad::eOk) return es;
    std::vector<Adesk::Int32> visibility = beam_viewable_;
    visibility.resize(vertices.length(), 1);
    beam_vertexes_ = vertices;
    top_offset_vertex_ = std::move(top);
    bottom_offset_vertex_ = std::move(bottom);
    beam_segment_direction_ = std::move(directions);
    beam_viewable_ = std::move(visibility);
    vertexes_num_ = vertices.length();
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::setBeamWidth(const double& width) {
    assertWriteEnabled();
    if (!std::isfinite(width) || width <= 0.0) return Acad::eInvalidInput;
    if (beam_vertexes_.length() >= 2) {
        AcGePoint3dArray top, bottom;
        AcGeVector3dArray directions;
        Acad::ErrorStatus es = BuildBeamGeometry(beam_vertexes_, width, top, bottom, directions);
        if (es != Acad::eOk) return es;
        top_offset_vertex_ = std::move(top);
        bottom_offset_vertex_ = std::move(bottom);
        beam_segment_direction_ = std::move(directions);
    }
    beam_b_ = width;
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::setBeamHeight(const double& height) {
    assertWriteEnabled();
    if (!std::isfinite(height) || height <= 0.0) return Acad::eInvalidInput;
    beam_h_ = height;
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::setBeamViewable(const std::vector<Adesk::Int32>& visible) {
    assertWriteEnabled();
    if (visible.size() != static_cast<size_t>(vertexes_num_)) return Acad::eInvalidInput;
    for (Adesk::Int32 value : visible)
        if (value != 0 && value != 1) return Acad::eInvalidInput;
    beam_viewable_ = visible;
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::setBeamProperty(const Adesk::Int32& property) {
    assertWriteEnabled();
    beam_property_ = property;
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::addVertexAt(int index, const AcGePoint3d& vertex) {
    assertWriteEnabled();
    if (index < 0 || index > vertexes_num_ || !Pola::BeamFinite(vertex)) return Acad::eInvalidInput;
    AcGePoint3dArray vertices = beam_vertexes_;
    vertices.insertAt(index, vertex);
    std::vector<Adesk::Int32> visible = beam_viewable_;
    visible.insert(visible.begin() + index, 1);
    Acad::ErrorStatus es = setBeamVertexes(vertices);
    if (es == Acad::eOk) beam_viewable_ = std::move(visible);
    return es;
}

Acad::ErrorStatus CPolaCustomBeam::addViewableAt(int index, Adesk::Int32 visible) {
    return resetViewableAt(index, visible);
}

Acad::ErrorStatus CPolaCustomBeam::resetViewableAt(int index, Adesk::Int32 visible) {
    assertWriteEnabled();
    if (index < 0 || index >= static_cast<int>(beam_viewable_.size()) ||
        (visible != 0 && visible != 1)) return Acad::eInvalidInput;
    beam_viewable_[index] = visible;
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::UpdateOffsetLine(const double& distance) {
    assertWriteEnabled();
    if (!std::isfinite(distance) || distance <= 0.0) return Acad::eInvalidInput;
    return setBeamWidth(distance * 2.0);
}

void CPolaCustomBeam::GenerateBeamSegmentDirection() {
    assertWriteEnabled();
    beam_segment_direction_.removeAll();
    if (beam_vertexes_.isEmpty()) return;
    beam_segment_direction_.append(AcGeVector3d::kIdentity);
    for (int i = 1; i < beam_vertexes_.length(); ++i)
        beam_segment_direction_.append(beam_vertexes_[i] - beam_vertexes_[i - 1]);
}

Acad::ErrorStatus CPolaCustomBeam::DrawBeamWithOffset(CPolaCustomBeam* beam,
    double offset, AcDbObjectId& id) {
    id = AcDbObjectId::kNull;
    if (!beam || !beam->objectId().isNull() || !std::isfinite(offset) ||
        !std::isfinite(beam->beam_b_) || beam->beam_b_ <= 0.0 ||
        !std::isfinite(beam->beam_h_) || beam->beam_h_ <= 0.0) return Acad::eInvalidInput;
    AcGePoint3d start, current;
    if (!SelectEntitys::PickPoint(_T("Pick first point:\n"), start) ||
        !SelectEntitys::PickPoint(_T("Pick next point:\n"), start, current))
        return Acad::eUserBreak;
    AcGePoint3d previous = current;
    AcGePoint3dArray vertices;
    AcGePoint3d offsetPoints[2];
    if (offset != 0.0) {
        if (!BasicTools::OffsetLineSegment(start, current, offset, offsetPoints)) return Acad::eInvalidInput;
        vertices.append(offsetPoints[0]);
        vertices.append(offsetPoints[1]);
    } else {
        vertices.append(start);
        vertices.append(current);
    }
    Adesk::Int32 visible = 1;
    Acad::ErrorStatus es = PickBeamVisibility(visible);
    if (es != Acad::eOk) return es;
    if ((es = beam->setBeamVertexes(vertices)) != Acad::eOk) return es;
    beam->beam_viewable_[0] = 0; // v1's unused leading visibility slot
    beam->beam_viewable_[1] = visible;
    // The UI owns the current handle. The database takes entity ownership here.
    if ((es = AddToModelSpace::AddEntityToModelSpace(beam, id, false)) != Acad::eOk)
        return RollbackBeamCreation(beam, id, es);
    while (SelectEntitys::PickPoint(_T("Pick next point (Enter finishes):\n"), previous, current)) {
        es = PickBeamVisibility(visible);
        if (es == Acad::eUserBreak) break;
        if (es != Acad::eOk) return RollbackBeamCreation(beam, id, es);
        AcGePoint3d vertex = current;
        if (offset != 0.0) {
            if (!BasicTools::OffsetLineSegment(previous, current, offset, offsetPoints)) {
                return RollbackBeamCreation(beam, id, Acad::eInvalidInput);
            }
            vertex = offsetPoints[1];
        }
        if ((es = beam->addVertexAt(beam->vertexes_num_, vertex)) != Acad::eOk) {
            return RollbackBeamCreation(beam, id, es);
        }
        beam->beam_viewable_.back() = visible;
        beam->recordGraphicsModified();
        acedUpdateDisplay();
        previous = current;
    }
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::PickCenterPointDrawBeam(CPolaCustomBeam* beam, AcDbObjectId& id) {
    return DrawBeamWithOffset(beam, 0.0, id);
}
Acad::ErrorStatus CPolaCustomBeam::PickTopPointDrawBeam(CPolaCustomBeam* beam, AcDbObjectId& id) {
    return DrawBeamWithOffset(beam, beam ? beam->beam_b_ * 0.5 : 0.0, id);
}
Acad::ErrorStatus CPolaCustomBeam::PickBottomPointDrawBeam(CPolaCustomBeam* beam, AcDbObjectId& id) {
    return DrawBeamWithOffset(beam, beam ? -beam->beam_b_ * 0.5 : 0.0, id);
}
AcDbObjectId CPolaCustomBeam::PickCenterPointDrawBeam(CPolaCustomBeam* beam) {
    AcDbObjectId id;
    ReportBeamError(PickCenterPointDrawBeam(beam, id));
    return id;
}
AcDbObjectId CPolaCustomBeam::PickTopPointDrawBeam(CPolaCustomBeam* beam) {
    AcDbObjectId id;
    ReportBeamError(PickTopPointDrawBeam(beam, id));
    return id;
}
AcDbObjectId CPolaCustomBeam::PickBottomPointDrawBeam(CPolaCustomBeam* beam) {
    AcDbObjectId id;
    ReportBeamError(PickBottomPointDrawBeam(beam, id));
    return id;
}

Acad::ErrorStatus CPolaCustomBeam::ModifyViewableChecked(CPolaCustomBeam* beam, int index, Adesk::Int32 visible) {
    if (!beam || index <= 0 || index >= beam->vertexes_num_) return Acad::eInvalidInput;
    Acad::ErrorStatus es = beam->resetViewableAt(index, visible);
    if (es != Acad::eOk) return es;
    beam->recordGraphicsModified();
    acedUpdateDisplay();
    return Acad::eOk;
}
Acad::ErrorStatus CPolaCustomBeam::ModifyViewableChecked(CPolaCustomBeam* beam, int index) {
    if (!beam || index <= 0 || index >= static_cast<int>(beam->beam_viewable_.size()))
        return Acad::eInvalidInput;
    return ModifyViewableChecked(beam, index, beam->beam_viewable_[index] ^ 1);
}
Acad::ErrorStatus CPolaCustomBeam::ModifyViewableChecked(AcDbObjectId id, int index, Adesk::Int32 visible) {
    CPolaCustomBeam* pointer = nullptr;
    Acad::ErrorStatus es = acdbOpenObject(pointer, id, AcDb::kForWrite);
    if (es != Acad::eOk) return es;
    Pola::OpenedObject<CPolaCustomBeam> beam(pointer);
    return Pola::Close(beam, ModifyViewableChecked(pointer, index, visible));
}
bool CPolaCustomBeam::ModifyViewable(CPolaCustomBeam* beam, int index, Adesk::Int32 visible) {
    Acad::ErrorStatus es = ModifyViewableChecked(beam, index, visible);
    ReportBeamError(es);
    return es == Acad::eOk;
}
bool CPolaCustomBeam::ModifyViewable(AcDbObjectId id, int index, Adesk::Int32 visible) {
    Acad::ErrorStatus es = ModifyViewableChecked(id, index, visible);
    ReportBeamError(es);
    return es == Acad::eOk;
}
bool CPolaCustomBeam::ModifyViewable(CPolaCustomBeam* beam, int index) {
    Acad::ErrorStatus es = ModifyViewableChecked(beam, index);
    ReportBeamError(es);
    return es == Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::SelectPillarDrawBeam(CPolaCustomBeam* beam, AcDbObjectId& id) {
    id = AcDbObjectId::kNull;
    if (!beam || !beam->objectId().isNull()) return Acad::eInvalidInput;
    AcDbObjectIdArray selected;
    if (!SelectEntitys::PickEntitys(_T("Select pillar\n"), CPolaCustomPillar::desc(), selected))
        return Acad::eUserBreak;
    if (selected.length() < 2) return Acad::eInvalidInput;
    AcGePoint3dArray vertices, sorted;
    for (AcDbObjectId pillarId : selected) {
        CPolaCustomPillar* pointer = nullptr;
        Acad::ErrorStatus es = acdbOpenObject(pointer, pillarId, AcDb::kForRead);
        if (es != Acad::eOk) return es;
        Pola::OpenedObject<CPolaCustomPillar> pillar(pointer);
        vertices.append(pillar->getCenterPoint());
        if ((es = Pola::Close(pillar)) != Acad::eOk) return es;
    }
    BasicTools::SortPointFromLeftToRight(vertices, sorted);
    Acad::ErrorStatus es = beam->setBeamVertexes(sorted);
    if (es != Acad::eOk) return es;
    beam->beam_viewable_.assign(beam->vertexes_num_, 1);
    es = AddToModelSpace::AddEntityToModelSpace(beam, id, false);
    return es == Acad::eOk ? es : RollbackBeamCreation(beam, id, es);
}
AcDbObjectId CPolaCustomBeam::SelectPillarDrawBeam(CPolaCustomBeam* beam) {
    AcDbObjectId id;
    ReportBeamError(SelectPillarDrawBeam(beam, id));
    return id;
}

Acad::ErrorStatus CPolaCustomBeam::subTransformBy(const AcGeMatrix3d& transform) {
    assertWriteEnabled();
    if (!HasDrawableGeometry()) return Acad::eInvalidInput;
    AcGePoint3dArray vertices = beam_vertexes_, top = top_offset_vertex_, bottom = bottom_offset_vertex_;
    for (int i = 0; i < vertexes_num_; ++i) {
        vertices[i].transformBy(transform);
        top[i].transformBy(transform);
        bottom[i].transformBy(transform);
        if (!Pola::BeamFinite(vertices[i]) || !Pola::BeamFinite(top[i]) || !Pola::BeamFinite(bottom[i]) ||
            (i > 0 && (vertices[i].isEqualTo(vertices[i - 1]) ||
                !Pola::BeamFinite(vertices[i] - vertices[i - 1])))) return Acad::eInvalidInput;
    }
    beam_vertexes_ = std::move(vertices);
    top_offset_vertex_ = std::move(top);
    bottom_offset_vertex_ = std::move(bottom);
    GenerateBeamSegmentDirection();
    recordGraphicsModified();
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subGetGeomExtents(AcDbExtents& extents) const {
    assertReadEnabled();
    extents = AcDbExtents();
    if (!HasDrawableGeometry()) return Acad::eInvalidInput;
    for (int i = 0; i < vertexes_num_; ++i) {
        extents.addPoint(beam_vertexes_[i]);
        extents.addPoint(top_offset_vertex_[i]);
        extents.addPoint(bottom_offset_vertex_[i]);
    }
    return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::GetIntersectingPillar(AcDbObjectIdArray& ids) const {
    assertReadEnabled();
    ids.removeAll();
    AcDbExtents beamExtents;
    Acad::ErrorStatus es = getGeomExtents(beamExtents);
    if (es != Acad::eOk) return es;
    AcDbObjectIdArray allPillars, result;
    es = SelectEntitys::GetAllEntitysByTypeChecked(CPolaCustomPillar::desc(), allPillars);
    if (es != Acad::eOk) return es;
    for (AcDbObjectId id : allPillars) {
        CPolaCustomPillar* pointer = nullptr;
        if ((es = acdbOpenObject(pointer, id, AcDb::kForRead)) != Acad::eOk) return es;
        Pola::OpenedObject<CPolaCustomPillar> pillar(pointer);
        AcDbExtents pillarExtents;
        es = pillar->getGeomExtents(pillarExtents);
        if (es == Acad::eOk && BasicTools::IsIntersectRectangle(beamExtents, pillarExtents))
            result.append(id);
        if ((es = Pola::Close(pillar, es)) != Acad::eOk) return es;
    }
    ids = std::move(result);
    return Acad::eOk;
}

AcDbObjectIdArray CPolaCustomBeam::GetIntersectingPillar() const {
    AcDbObjectIdArray ids;
    ReportBeamError(GetIntersectingPillar(ids));
    return ids;
}

Adesk::Int32 CPolaCustomBeam::GetSegmentIndexByYProjection(const AcGePoint3d & point, const AcGeTol & tol) const
{
	if (!Pola::BeamFinite(point)) return -1;
	Adesk::Int32 segment_index = -1;
	double min_t = DBL_MAX;

	const AcGePoint3dArray& vertices = beam_vertexes_;
	const int vertexes_count = vertices.length() - 1;

	if (vertexes_count < 1)
		return -1;

	for (int i = 0; i < vertexes_count; ++i)
	{
		const AcGePoint3d& v1 = vertices[i];
		const AcGePoint3d& v2 = vertices[i + 1];

		const bool isVertical = (fabs(v2.x - v1.x) <= tol.equalPoint());

		if (isVertical)
		{

			if (fabs(v1.x - point.x) > tol.equalPoint())
				continue;

			const double y_min = BasicTools::Min(v1.y, v2.y);
			const double y_max = BasicTools::Max(v1.y, v2.y);

			const double t = point.y - y_max;

			if (t >= -tol.equalVector())
			{

				if (t < min_t)
				{
					min_t = t;
					segment_index = i;
				}
			}
		}
		else
		{
			const double dx = v2.x - v1.x;
			const double dy = v2.y - v1.y;

			const double s = (point.x - v1.x) / dx;

			if (s < -tol.equalPoint() || s > 1.0 + tol.equalPoint())
				continue;

			const double y_intersect = v1.y + s * dy;
			const double t = point.y - y_intersect;

			if (t < -tol.equalVector())
				continue;

			const double z_intersect = v1.z + s * (v2.z - v1.z);
			if (fabs(z_intersect - point.z) > tol.equalPoint())
				continue;

			if (t < min_t)
			{
				min_t = t;
				segment_index = i;
			}
		}
	}
	return segment_index;
}

Adesk::Int32 CPolaCustomBeam::GetSegmentIndex(const AcGePoint3d & point, const AcGeTol & tol) const
{
	if (!Pola::BeamFinite(point) || beam_vertexes_.length() < 2)
		return -1;
	double min_distance = DBL_MAX;
	int closest_segment_index = -1;

	for (int i = 0; i < beam_vertexes_.length() - 1; i++)
	{
		AcGePoint3d start_point = beam_vertexes_[i];
		AcGePoint3d end_point = beam_vertexes_[i + 1];

		AcGeLineSeg3d segment(start_point, end_point);

		AcGePoint3d closest_point;
		closest_point = segment.closestPointTo(point);

		if (segment.isOn(closest_point, tol))
		{
			double dist = point.distanceTo(closest_point);
			if (dist < min_distance)
			{
				min_distance = dist;
				closest_segment_index = i;
			}
		}
	}
	if (closest_segment_index == -1)
	{
		for (int i = 0; i < beam_vertexes_.length(); i++)
		{
			double dist = point.distanceTo(beam_vertexes_[i]);
			if (dist < min_distance)
			{
				min_distance = dist;
				closest_segment_index = (i == beam_vertexes_.length() - 1) ? i - 1 : i;
			}
		}
	}
	return closest_segment_index;
}

Acad::ErrorStatus CPolaCustomBeam::addJoint(double slab_thickness, double offset_length, AcDbObjectIdArray& joint_line_ids)
{
    joint_line_ids.removeAll();
    if (!HasDrawableGeometry() || !std::isfinite(slab_thickness) || slab_thickness < 0.0 ||
        !std::isfinite(offset_length) || offset_length < 0.0) return Acad::eInvalidInput;
    AcDbObjectIdArray intersecting_pillar_ids;
    Acad::ErrorStatus status = GetIntersectingPillar(intersecting_pillar_ids);
    if (status != Acad::eOk) return status;
    std::vector<std::unique_ptr<AcDbLine>> lines;

    auto CreateLine = [&lines, &status](const AcGePoint3d& start, const AcGePoint3d& end,
        const ACHAR* line_type, double scale = 1.0) {
        if (status != Acad::eOk) return;
        if (!Pola::BeamFinite(start) || !Pola::BeamFinite(end)) {
            status = Acad::eInvalidInput;
            return;
        }
        std::unique_ptr<AcDbLine> line(new AcDbLine(start, end));
        AcDbObjectId typeId;
        if ((status = StyleTools::GetLineStyleId(line_type, typeId)) != Acad::eOk) return;
        if ((status = line->setLinetype(typeId)) != Acad::eOk) return;
        if ((status = line->setLinetypeScale(scale)) != Acad::eOk) return;
        lines.push_back(std::move(line));
    };

	for (int i = 1; i < beam_vertexes_.length() - 1; i++)
	{
		if (beam_viewable_.at(i) == beam_viewable_.at(i + 1))
		{
			continue;
		}

		const AcGePoint3d& vertex = beam_vertexes_.at(i);

		for (const auto& pillar_id : intersecting_pillar_ids)
		{
			AcDbEntity* pillar_entity = nullptr;
			if ((status = acdbOpenObject(pillar_entity, pillar_id, AcDb::kForRead)) != Acad::eOk)
                return status;

			Pola::OpenedObject<AcDbEntity> pillar_guard(pillar_entity);

			CPolaCustomPillar* pillar = CPolaCustomPillar::cast(pillar_entity);
			if (!pillar) return Pola::Close(pillar_guard, Acad::eWrongObjectType);
            if (pillar->getCenterPoint() != vertex) {
                if ((status = Pola::Close(pillar_guard)) != Acad::eOk) return status;
                continue;
            }

			double pillar_b, pillar_h;
			pillar->getDiameter(pillar_b, pillar_h);
            if (pillar->getPillarType() == 0) pillar_h = pillar_b;
            if (!std::isfinite(pillar_b) || !std::isfinite(pillar_h) || pillar_b <= 0 || pillar_h <= 0)
                return Pola::Close(pillar_guard, Acad::eInvalidInput);

			bool isHorizontalBeam = true;
			if (i > 0 && i < beam_vertexes_.length() - 1)
			{
				double dx = fabs(beam_vertexes_[i + 1].x - beam_vertexes_[i - 1].x);
				double dy = fabs(beam_vertexes_[i + 1].y - beam_vertexes_[i - 1].y);
				isHorizontalBeam = (dx >= dy);
			}

			AcGePoint3d p1, p2, p3, p4, p5, p6, p7, p8;

			if (isHorizontalBeam)
			{
				p1 = AcGePoint3d(vertex.x - pillar_b / 2 - offset_length - beam_h_ + slab_thickness, vertex.y + beam_b_ / 2, vertex.z);
				p2 = AcGePoint3d(vertex.x - pillar_b / 2 - offset_length - beam_h_ + slab_thickness, vertex.y - beam_b_ / 2, vertex.z);

				p3 = AcGePoint3d(vertex.x - pillar_b / 2 - offset_length, vertex.y + beam_b_ / 2, vertex.z);
				p4 = AcGePoint3d(vertex.x - pillar_b / 2 - offset_length, vertex.y - beam_b_ / 2, vertex.z);

				p5 = AcGePoint3d(vertex.x + pillar_b / 2 + offset_length, vertex.y + beam_b_ / 2, vertex.z);
				p6 = AcGePoint3d(vertex.x + pillar_b / 2 + offset_length, vertex.y - beam_b_ / 2, vertex.z);

				p7 = AcGePoint3d(vertex.x + pillar_b / 2 + offset_length + beam_h_ - slab_thickness, vertex.y + beam_b_ / 2, vertex.z);
				p8 = AcGePoint3d(vertex.x + pillar_b / 2 + offset_length + beam_h_ - slab_thickness, vertex.y - beam_b_ / 2, vertex.z);
			}
			else
			{
				p1 = AcGePoint3d(vertex.x + beam_b_ / 2, vertex.y - pillar_h / 2 - offset_length - beam_h_ + slab_thickness, vertex.z);
				p2 = AcGePoint3d(vertex.x - beam_b_ / 2, vertex.y - pillar_h / 2 - offset_length - beam_h_ + slab_thickness, vertex.z);

				p3 = AcGePoint3d(vertex.x + beam_b_ / 2, vertex.y - pillar_h / 2 - offset_length, vertex.z);
				p4 = AcGePoint3d(vertex.x - beam_b_ / 2, vertex.y - pillar_h / 2 - offset_length, vertex.z);

				p5 = AcGePoint3d(vertex.x + beam_b_ / 2, vertex.y + pillar_h / 2 + offset_length, vertex.z);
				p6 = AcGePoint3d(vertex.x - beam_b_ / 2, vertex.y + pillar_h / 2 + offset_length, vertex.z);

				p7 = AcGePoint3d(vertex.x + beam_b_ / 2, vertex.y + pillar_h / 2 + offset_length + beam_h_ - slab_thickness, vertex.z);
				p8 = AcGePoint3d(vertex.x - beam_b_ / 2, vertex.y + pillar_h / 2 + offset_length + beam_h_ - slab_thickness, vertex.z);
			}

			if (beam_viewable_.at(i) == 1)
			{
				CreateLine(p1, p2, _T("DASHED"), 700);
				CreateLine(p3, p4, _T("DASHED"), 700);
				CreateLine(p5, p6, _T("CONTINUOUS"));
				CreateLine(p7, p8, _T("CONTINUOUS"));
				CreateLine(p3, p7, _T("CONTINUOUS"));
				CreateLine(p4, p8, _T("CONTINUOUS"));
			}
			else
			{
				CreateLine(p1, p2, _T("CONTINUOUS"));
				CreateLine(p3, p4, _T("CONTINUOUS"));
				CreateLine(p5, p6, _T("DASHED"), 700);
				CreateLine(p7, p8, _T("DASHED"), 700);
				CreateLine(p1, p5, _T("CONTINUOUS"));
				CreateLine(p2, p6, _T("CONTINUOUS"));
			}
            if ((status = Pola::Close(pillar_guard, status)) != Acad::eOk) return status;
			break;
		}
	}
    if (status != Acad::eOk) return status;
    AcDbObjectIdArray inserted;
    for (auto& pending : lines) {
        AcDbObjectId id;
        status = AddToModelSpace::AddEntityToModelSpace(pending.get(), id, false);
        if (!pending->objectId().isNull()) {
            inserted.append(pending->objectId());
            Pola::OpenedObject<AcDbLine> handle(pending.release());
            status = Pola::Close(handle, status);
        }
        if (status != Acad::eOk) {
            // Restore pre-operation database contents on a partially appended joint.
            for (AcDbObjectId insertedId : inserted) {
                AcDbEntity* pointer = nullptr;
                Acad::ErrorStatus cleanup = acdbOpenObject(pointer, insertedId, AcDb::kForWrite);
                if (cleanup == Acad::eOk) {
                    Pola::OpenedObject<AcDbEntity> entity(pointer);
                    cleanup = Pola::Close(entity, entity->erase());
                }
                if (cleanup != Acad::eOk)
                    acutPrintf(_T("\nJoint rollback failed: %d\n"), static_cast<int>(cleanup));
            }
            return status;
        }
    }
    joint_line_ids = std::move(inserted);
    return Acad::eOk;
}

AcDbObjectIdArray CPolaCustomBeam::addJoint(double slab, double offset) {
    AcDbObjectIdArray ids;
    ReportBeamError(addJoint(slab, offset, ids));
    return ids;
}

Acad::ErrorStatus CPolaCustomBeam::addBeamSnInfo(AcDbObjectId& id) {
    assertReadEnabled();
    id = AcDbObjectId::kNull;
    if (!HasDrawableGeometry()) return Acad::eInvalidInput;
    const int segment = GetSegmentIndex(getHorizontalMidPoint());
    if (segment < 0 || segment + 1 >= vertexes_num_) return Acad::eInvalidInput;
    const AcGePoint3d insertion = BasicTools::GetMidPoint(beam_vertexes_[segment],
        beam_vertexes_[segment + 1]) + AcGeVector3d(0, beam_b_, 0);
    std::wstringstream info;
    info << _T("KL") << beam_property_ << _T(" ") << std::trunc(beam_b_) << _T("*") << std::trunc(beam_h_);
    std::unique_ptr<AcDbText> text(new AcDbText());
    Acad::ErrorStatus es = text->setPosition(insertion);
    if (es != Acad::eOk) return es;
    if ((es = text->setTextString(info.str().c_str())) != Acad::eOk) return es;
    if ((es = text->setHeight(450)) != Acad::eOk) return es;
    if ((es = text->setWidthFactor(0.7)) != Acad::eOk) return es;
    const AcDbObjectId style = StyleTools::InitTextStyle();
    if (style.isNull()) return Acad::eKeyNotFound;
    if ((es = text->setTextStyle(style)) != Acad::eOk) return es;
    if ((es = text->setHorizontalMode(AcDb::kTextCenter)) != Acad::eOk) return es;
    if ((es = text->setAlignmentPoint(insertion)) != Acad::eOk) return es;
    if ((es = AddToModelSpace::AddEntityToModelSpace(text.get(), id, false)) != Acad::eOk) {
        if (!text->objectId().isNull()) {
            Pola::OpenedObject<AcDbText> handle(text.release());
            const Acad::ErrorStatus cleanup = handle->erase();
            if (cleanup != Acad::eOk) ReportBeamError(cleanup);
            return Pola::Close(handle, es);
        }
        return es;
    }
    Pola::OpenedObject<AcDbText> handle(text.release());
    es = handle->adjustAlignment(acdbHostApplicationServices()->workingDatabase());
    if (es != Acad::eOk) {
        const Acad::ErrorStatus cleanup = handle->erase();
        if (cleanup != Acad::eOk) ReportBeamError(cleanup);
    }
    es = Pola::Close(handle, es);
    if (es != Acad::eOk) id = AcDbObjectId::kNull;
    return es;
}

AcDbObjectId CPolaCustomBeam::addBeamSnInfo() {
    AcDbObjectId id;
    ReportBeamError(addBeamSnInfo(id));
    return id;
}

Acad::ErrorStatus CPolaCustomBeam::InsertVertex(const AcGePoint3d& point) {
    assertWriteEnabled();
    if (!HasDrawableGeometry() || !Pola::BeamFinite(point)) return Acad::eInvalidInput;
    const int index = GetSegmentIndex(point);
    if (index < 0) return Acad::eInvalidInput;
    Acad::ErrorStatus es = addVertexAt(index + 1, point);
    if (es != Acad::eOk) return es;
    recordGraphicsModified();
    acedUpdateDisplay();
    return Acad::eOk;
}
