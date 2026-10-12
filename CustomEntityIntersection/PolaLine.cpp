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
//----- PolaLine.cpp : Implementation of CPolaLine
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PolaLine.h"
#include <cmath>

namespace {
bool validVertex(const AcGePoint3d& point) {
	return std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z);
}
bool validVertices(const AcGePoint3dArray& points) {
	if (points.length() == 1 || points.length() > 1000000) return false;
	for (int i = 0; i < points.length(); ++i) {
		if (!validVertex(points[i])) return false;
		if (i > 0 && points[i].isEqualTo(points[i - 1])) return false;
	}
	return true;
}
}

//-----------------------------------------------------------------------------
Adesk::UInt32 CPolaLine::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	CPolaLine, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, POLALINE,
	POLACUSTOMENTITYINTERSECTIONAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
CPolaLine::CPolaLine() : AcDbEntity() {
}

CPolaLine::~CPolaLine() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CPolaLine::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	if (!pFiler || !validVertices(center_vertex_) || center_num_ != center_vertex_.length()
		|| !std::isfinite(line_width_) || line_width_ < 0.0) return Acad::eInvalidInput;
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(CPolaLine::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	es = pFiler->writeItem(center_num_);
	if (es != Acad::eOk) return es;
	es = pFiler->writeItem(line_width_);
	if (es != Acad::eOk) return es;

	for (int i = 0;i < center_num_;i++)
	{
		es = pFiler->writeItem(center_vertex_.at(i));
		if (es != Acad::eOk) return es;
	}

	return (pFiler->filerStatus());
}

Acad::ErrorStatus CPolaLine::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	if (!pFiler) return Acad::eInvalidInput;
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version != CPolaLine::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CPolaLine::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	Adesk::Int32 next_count = 0;
	double next_width = 0.0;
	AcGePoint3dArray next_vertices;
	es = pFiler->readItem(&next_count);
	if (es != Acad::eOk) return es;
	if (next_count < 0 || next_count == 1 || next_count > 1000000) return Acad::eInvalidInput;
	es = pFiler->readItem(&next_width);
	if (es != Acad::eOk) return es;
	if (!std::isfinite(next_width) || next_width < 0.0) return Acad::eInvalidInput;

	for (int i = 0;i < next_count;i++)
	{
		AcGePoint3d vertex;
		es = pFiler->readItem(&vertex);
		if (es != Acad::eOk) return es;
		next_vertices.append(vertex);
	}
	if ((es = pFiler->filerStatus()) != Acad::eOk) return es;
	if (!validVertices(next_vertices)) return Acad::eInvalidInput;
	center_num_ = next_count;
	line_width_ = next_width;
	center_vertex_ = next_vertices;
	line_seg.clear();
	return Acad::eOk;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaLine::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	AcGePoint3dArray top, bot;
	GenerateOffsetLine(top, bot);

	return (AcDbEntity::subWorldDraw(mode));
}


Adesk::UInt32 CPolaLine::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

//- Osnap points protocol
Acad::ErrorStatus CPolaLine::subGetOsnapPoints(
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d & pickPoint,
	const AcGePoint3d & lastPoint,
	const AcGeMatrix3d & viewXform,
	AcGePoint3dArray & snapPoints,
	AcDbIntArray & geomIds) const
{
	assertReadEnabled();
	return (AcDbEntity::subGetOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds));
}

Acad::ErrorStatus CPolaLine::subGetOsnapPoints(
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d & pickPoint,
	const AcGePoint3d & lastPoint,
	const AcGeMatrix3d & viewXform,
	AcGePoint3dArray & snapPoints,
	AcDbIntArray & geomIds,
	const AcGeMatrix3d & insertionMat) const
{
	assertReadEnabled();
	return (AcDbEntity::subGetOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat));
}

//- Grip points protocol
Acad::ErrorStatus CPolaLine::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	//----- This method is never called unless you return eNotImplemented
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints(gripPoints, osnapModes, geomIds));
}

Acad::ErrorStatus CPolaLine::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset) {
	assertWriteEnabled();
	//----- This method is never called unless you return eNotImplemented
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt(indices, offset));
}

Acad::ErrorStatus CPolaLine::subGetGripPoints(
	AcDbGripDataPtrArray & grips, const double curViewUnitSize, const int gripSize,
	const AcGeVector3d & curViewDir, const int bitflags
) const {
	assertReadEnabled();

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints(grips, curViewUnitSize, gripSize, curViewDir, bitflags));
}

Acad::ErrorStatus CPolaLine::subMoveGripPointsAt(
	const AcDbVoidPtrArray & gripAppData, const AcGeVector3d & offset,
	const int bitflags
) {
	assertWriteEnabled();

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt(gripAppData, offset, bitflags));
}

void CPolaLine::SetCenterVertex(const AcGePoint3dArray & center_vertex)
{
	const Acad::ErrorStatus status = SetCenterVertexChecked(center_vertex);
	if (status != Acad::eOk) acutPrintf(_T("\nLine vertices invalid: %d"), static_cast<int>(status));
}

Acad::ErrorStatus CPolaLine::SetCenterVertexChecked(const AcGePoint3dArray& center_vertex)
{
	assertWriteEnabled();
	if (!validVertices(center_vertex)) return Acad::eInvalidInput;
	center_vertex_ = center_vertex;
	center_num_ = center_vertex_.length();
	line_seg.clear();
	return Acad::eOk;
}

void CPolaLine::GenerateOffsetLine(AcGePoint3dArray & top_line, AcGePoint3dArray & bottom_line)
{
	const Acad::ErrorStatus status = GenerateOffsetLineChecked(top_line, bottom_line);
	if (status != Acad::eOk) acutPrintf(_T("\nLine offset failed: %d"), static_cast<int>(status));
}

Acad::ErrorStatus CPolaLine::GenerateOffsetLineChecked(AcGePoint3dArray& top_line, AcGePoint3dArray& bottom_line)
{
	assertReadEnabled();
	top_line.removeAll();
	bottom_line.removeAll();
	if (center_vertex_.length() < 2 || !validVertices(center_vertex_) || !std::isfinite(line_width_) || line_width_ < 0.0)
		return Acad::eInvalidInput;
	AcGePoint3dArray top, bottom;
	for (int i = 0; i < center_vertex_.length(); ++i)
	{
		AcGeVector3d offset;
		if (i == 0 || i == center_vertex_.length() - 1)
		{
			AcGeVector3d dir;
			if (i == 0)
				dir = center_vertex_[1] - center_vertex_[0];
			else
				dir = center_vertex_[i] - center_vertex_[i - 1];

			const double length = std::hypot(dir.x, dir.y);
			if (!std::isfinite(length) || length <= AcGeContext::gTol.equalPoint()) return Acad::eInvalidInput;
			offset = AcGeVector3d(-dir.y / length, dir.x / length, 0.0) * (line_width_ / 2);
		}
		else
		{
			const AcGeVector3d prev = center_vertex_[i] - center_vertex_[i - 1];
			const AcGeVector3d next = center_vertex_[i + 1] - center_vertex_[i];
			const double prev_length = std::hypot(prev.x, prev.y), next_length = std::hypot(next.x, next.y);
			if (!std::isfinite(prev_length) || !std::isfinite(next_length)
				|| prev_length <= AcGeContext::gTol.equalPoint() || next_length <= AcGeContext::gTol.equalPoint()) return Acad::eInvalidInput;
			const AcGeVector3d prev_normal(-prev.y / prev_length, prev.x / prev_length, 0.0);
			const AcGeVector3d next_normal(-next.y / next_length, next.x / next_length, 0.0);
			const AcGeVector3d bisector = prev_normal + next_normal;
			if (bisector.length() <= AcGeContext::gTol.equalVector()) return Acad::eInvalidInput;
			const AcGeVector3d unit = bisector.normal();
			const double denominator = unit.dotProduct(prev_normal);
			if (std::abs(denominator) <= AcGeContext::gTol.equalVector()) return Acad::eInvalidInput;
			offset = unit * (line_width_ / 2 / denominator);
		}
		const AcGePoint3d top_point = center_vertex_[i] + offset, bottom_point = center_vertex_[i] - offset;
		if (!validVertex(top_point) || !validVertex(bottom_point)) return Acad::eInvalidInput;
		top.append(top_point);
		bottom.append(bottom_point);
	}
	top_line = top;
	bottom_line = bottom;
	return Acad::eOk;
}

void CPolaLine::GenerateOriginalDrawSegment()
{
	line_seg.clear();
	int cnt = center_vertex_.length();
	AcGePoint3dArray top, bot;
	if (GenerateOffsetLineChecked(top, bot) != Acad::eOk) return;
	for (int i = 0;i < cnt - 1;i++)
	{
		DrawSegment seg;
		seg.top_start_point = top.at(i);
		seg.top_end_point = top.at(i + 1);
		seg.bottom_start_point = bot.at(i);
		seg.bottom_end_point = bot.at(i + 1);

		seg.type = LineType::SOLID;
		line_seg.push_back(seg);
	}
}

bool CPolaLine::ModifyDrawSegmentAt(int index, CPolaLine::LineType type)
{
	assertWriteEnabled();
	if (type != SOLID && type != DASHED && type != HIDDEN) return false;
	if (index < 0 || index >= line_seg.size())
		return false;

	line_seg.at(index).type = type;
	return true;
}

void CPolaLine::CheckCollision(CPolaLine & other)
{
	GenerateOriginalDrawSegment();
	other.GenerateOriginalDrawSegment();

	for (auto it_this = line_seg.begin();it_this != line_seg.end();it_this++)
	{
		for (auto it_other = other.line_seg.begin();it_other != other.line_seg.end();it_other++)
		{
			AcGeLine3d this_top(it_this->top_start_point, it_this->top_end_point);
			AcGeLine3d this_bot(it_this->bottom_start_point, it_this->bottom_end_point);

			AcGeLine3d other_top(it_other->top_start_point, it_other->top_end_point);
			AcGeLine3d other_bot(it_other->bottom_start_point, it_other->bottom_end_point);

			AcGePoint3d top_start_point;
			if (!this_top.intersectWith(other_top, top_start_point))
				continue;
			AcGePoint3d bot_end_point;
			if (!this_top.intersectWith(other_bot, bot_end_point))
				continue;
		}
	}
}

int CPolaLine::GetSegmentIndex(const AcGePoint3d point)
{
	assertReadEnabled();
	if (!validVertex(point) || center_vertex_.length() < 2 || !validVertices(center_vertex_))
		return -1;
	double min_distance = DBL_MAX;
	int closest_segment_index = -1;

	for (int i = 0; i < center_vertex_.length() - 1; i++)
	{
		AcGePoint3d start_point = center_vertex_[i];
		AcGePoint3d end_point = center_vertex_[i + 1];

		AcGeLineSeg3d segment(start_point, end_point);

		AcGePoint3d closest_point;
		closest_point = segment.closestPointTo(point);

		if (segment.isOn(closest_point, AcGeContext::gTol))
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
		for (int i = 0; i < center_vertex_.length(); i++)
		{
			double dist = point.distanceTo(center_vertex_[i]);
			if (dist < min_distance)
			{
				min_distance = dist;
				closest_segment_index = (i == center_vertex_.length() - 1) ? i - 1 : i;
			}
		}
	}
	return closest_segment_index;
}
