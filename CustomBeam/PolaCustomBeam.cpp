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
Acad::ErrorStatus CPolaCustomBeam::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(CPolaCustomBeam::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	es = pFiler->writeItem(vertexes_num_);
	if (es != Acad::eOk)
		return es;
	for (int i = 0; i < vertexes_num_;i++)
	{
		es = pFiler->writeItem(beam_vertexes_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	for (int i = 0;i < vertexes_num_;i++)
	{
		es = pFiler->writeItem(top_offset_vertex_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	for (int i = 0;i < vertexes_num_;i++)
	{
		es = pFiler->writeItem(bottom_offset_vertex_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	es = pFiler->writeItem(beam_b_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(beam_h_);
	if (es != Acad::eOk)
		return es;
	for (int i = 0; i < vertexes_num_;i++)
	{
		es = pFiler->writeItem(beam_viewable_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	es = pFiler->writeItem(beam_property_);
	if (es != Acad::eOk)
		return es;
	return (pFiler->filerStatus());
}

Acad::ErrorStatus CPolaCustomBeam::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > CPolaCustomBeam::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CPolaCustomBeam::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	es = pFiler->readItem(&vertexes_num_);
	if (es != Acad::eOk)
		return es;
	if (vertexes_num_ < 2)
		return Acad::eInvalidInput;
	beam_vertexes_.setLogicalLength(vertexes_num_);
	top_offset_vertex_.setLogicalLength(vertexes_num_);
	bottom_offset_vertex_.setLogicalLength(vertexes_num_);
	for (int i = 0; i < vertexes_num_; i++)
	{
		es = pFiler->readItem(&beam_vertexes_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	for (int i = 0; i < vertexes_num_; i++)
	{
		es = pFiler->readItem(&top_offset_vertex_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	for (int i = 0; i < vertexes_num_; i++)
	{
		es = pFiler->readItem(&bottom_offset_vertex_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	es = pFiler->readItem(&beam_b_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&beam_h_);
	if (es != Acad::eOk)
		return es;
	beam_viewable_.resize(vertexes_num_);
	for (int i = 0; i < vertexes_num_; i++)
	{
		es = pFiler->readItem(&beam_viewable_.at(i));
		if (es != Acad::eOk)
			return es;
	}
	es = pFiler->readItem(&beam_property_);
	if (es != Acad::eOk)
		return es;
	return (pFiler->filerStatus());
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaCustomBeam::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	if (vertexes_num_ < 2)
	{
		return AcDbEntity::subWorldDraw(mode);
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
			mode->subEntityTraits().setSelectionMarker(base_marker);
			temp_top_line.worldDraw(mode);
			mode->subEntityTraits().setSelectionMarker(base_marker + 2);
			temp_bottom_line.worldDraw(mode);

			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CENTER")));
			mode->subEntityTraits().setSelectionMarker(base_marker + 1);
			temp_center_line.worldDraw(mode);
		}
		else if (beam_viewable_.at(i + 1) == 1)
		{
			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
			mode->subEntityTraits().setSelectionMarker(base_marker);
			temp_top_line.worldDraw(mode);
			mode->subEntityTraits().setSelectionMarker(base_marker + 2);
			temp_bottom_line.worldDraw(mode);

			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CENTER")));
			mode->subEntityTraits().setLineTypeScale(700);
			mode->subEntityTraits().setSelectionMarker(base_marker + 1);
			temp_center_line.worldDraw(mode);
		}
		else
		{
			throw;
		}
	}
	
	AcDbLine left_side_line(top_offset_vertex_.first(), bottom_offset_vertex_.first());
	AcDbLine right_side_line(top_offset_vertex_.last(), bottom_offset_vertex_.last());
	left_side_line.worldDraw(mode);
	right_side_line.worldDraw(mode);
	return (AcDbEntity::subWorldDraw(mode));
}

Adesk::UInt32 CPolaCustomBeam::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

//- Osnap points protocol
Acad::ErrorStatus CPolaCustomBeam::subGetOsnapPoints(
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d & pickPoint,
	const AcGePoint3d & lastPoint,
	const AcGeMatrix3d & viewXform,
	AcGePoint3dArray & snapPoints,
	AcDbIntArray & geomIds) const
{
	assertReadEnabled();
	if (gsSelectionMark == -1)
	{
		AcDbEntity::subGetOsnapPoints(
			osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
	}

	int segment_index = static_cast<int>(gsSelectionMark / 3);
	int subordinate_line_type = gsSelectionMark % 3;

	Acad::ErrorStatus error_status;
	AcDbLine* subordinate_line_segment = nullptr;

	switch (subordinate_line_type)
	{
	case 0:
		subordinate_line_segment = new AcDbLine(
			top_offset_vertex_.at(segment_index), top_offset_vertex_.at(segment_index + 1));
		break;
	case 1:
		subordinate_line_segment = new AcDbLine(
			beam_vertexes_.at(segment_index), beam_vertexes_.at(segment_index + 1));
		break;
	case 2:
		subordinate_line_segment = new AcDbLine(
			bottom_offset_vertex_.at(segment_index), bottom_offset_vertex_.at(segment_index + 1));
		break;
	default:
		return Acad::eInvalidInput;
	}
	error_status = subordinate_line_segment->getOsnapPoints(
		osnapMode, -1, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
	if (error_status != Acad::eOk)
		return error_status;
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subGetOsnapPoints(
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
	if (gsSelectionMark == -1)
	{
		AcDbEntity::subGetOsnapPoints(
			osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
	}

	int segment_index = static_cast<int>(gsSelectionMark / 3);
	int subordinate_line_type = gsSelectionMark % 3;

	Acad::ErrorStatus error_status;
	AcDbLine* subordinate_line_segment = nullptr;

	switch (subordinate_line_type)
	{
	case 0:
		subordinate_line_segment = new AcDbLine(
			top_offset_vertex_.at(segment_index), top_offset_vertex_.at(segment_index + 1));
		break;
	case 1:
		subordinate_line_segment = new AcDbLine(
			beam_vertexes_.at(segment_index), beam_vertexes_.at(segment_index + 1));
		break;
	case 2:
		subordinate_line_segment = new AcDbLine(
			bottom_offset_vertex_.at(segment_index), bottom_offset_vertex_.at(segment_index + 1));
		break;
	default:
		return Acad::eInvalidInput;
	}
	error_status = subordinate_line_segment->getOsnapPoints(
		osnapMode, -1, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
	if (error_status != Acad::eOk)
		return error_status;

	return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus CPolaCustomBeam::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	//return (AcDbEntity::subGetGripPoints(gripPoints, osnapModes, geomIds));
	for (int i = 0;i < vertexes_num_;i++)
	{
		gripPoints.append(beam_vertexes_.at(i));
	}
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset) {
	assertWriteEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt(indices, offset));
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

Adesk::Int32 CPolaCustomBeam::getBeamProperty() const
{
	assertReadEnabled();
	return beam_property_;
}

Adesk::Int32 CPolaCustomBeam::getVertexesNum() const
{
	return beam_vertexes_.length();
}

double CPolaCustomBeam::getBeamLength() const
{
	double length = 0;
	for (int i = 0;i < vertexes_num_ - 1;i++)
	{
		length += beam_vertexes_.at(i).distanceTo(beam_vertexes_.at(i + 1));
	}
	return length;
}

AcGePoint3d CPolaCustomBeam::getHorizontalMidPoint() const
{
	if (beam_vertexes_.length() < 2)
	{
		return AcGePoint3d::kOrigin;
	}

	std::vector<double> segment_lengths;
	double total_length = 0.0;
	for (int i = 0; i < beam_vertexes_.length() - 1; ++i)
	{
		double len = beam_vertexes_[i].distanceTo(beam_vertexes_[i + 1]);
		segment_lengths.push_back(len);
		total_length += len;
	}
	double middle_position = total_length / 2.0;

	double current_sum = 0.0;
	AcGePoint3d path_mid_point;
	bool isMidOnHorizontal = false;
	int middle_segment_index = -1;

	for (int i = 0; i < segment_lengths.size(); ++i)
	{
		double segment_length = segment_lengths[i];
		if (current_sum + segment_length >= middle_position)
		{
			double remain = middle_position - current_sum;
			AcGePoint3d start = beam_vertexes_[i];
			AcGePoint3d end = beam_vertexes_[i + 1];
			double ratio = remain / segment_length;
			AcGeVector3d vec = end - start;
			vec *= ratio;
			path_mid_point = start + vec;

			isMidOnHorizontal = (fabs(start.y - end.y) < 1e-10);
			middle_segment_index = i;
			break;
		}
		current_sum += segment_length;
	}

	if (isMidOnHorizontal)
	{
		return path_mid_point;
	}

	std::vector<std::pair<double, AcGePoint3d>> horizontal_segments;
	current_sum = 0.0;
	for (int i = 0; i < segment_lengths.size(); ++i)
	{
		AcGePoint3d start = beam_vertexes_[i];
		AcGePoint3d end = beam_vertexes_[i + 1];
		double segment_len = segment_lengths[i];

		if (fabs(start.y - end.y) < 1e-10)
		{
			AcGePoint3d mid_point(
				(start.x + end.x) / 2.0,
				(start.y + end.y) / 2.0,
				(start.z + end.z) / 2.0
			);
			double mid_path_position = current_sum + segment_len / 2.0;
			horizontal_segments.emplace_back(mid_path_position, mid_point);
		}
		current_sum += segment_len;
	}

	if (horizontal_segments.empty())
	{
		return path_mid_point;
	}

	double min_difference = (std::numeric_limits<double>::max)();

	AcGePoint3d horizontal_mid_point = path_mid_point;
	for (const auto& horizontal_segment_data : horizontal_segments)
	{
		double diff = fabs(horizontal_segment_data.first - middle_position);
		if (diff < min_difference)
		{
			min_difference = diff;
			horizontal_mid_point = horizontal_segment_data.second;
		}
	}
	return horizontal_mid_point;
}

void CPolaCustomBeam::setBeamVertexes(const AcGePoint3dArray & beam_vertexes)
{
	assertWriteEnabled();
	if (!beam_vertexes.isEmpty())
	{
		for (int i = 0; i < beam_vertexes.length(); i++)
		{
			beam_vertexes_.append(beam_vertexes.at(i));
		}
	}
	vertexes_num_ = beam_vertexes.length();
}

void CPolaCustomBeam::setBeamWidth(const double& beam_b)
{
	assertWriteEnabled();
	beam_b_ = beam_b;
}

void CPolaCustomBeam::setBeamHeight(const double& beam_h)
{
	assertWriteEnabled();
	beam_h_ = beam_h;
}

void CPolaCustomBeam::setBeamViewable(const std::vector<Adesk::Int32>&beam_viewable)
{
	assertWriteEnabled();
	if (beam_viewable.size() != 0)
	{
		for (int i = 0; i < beam_viewable.size(); i++)
		{
			beam_viewable_.push_back(beam_viewable.at(i));
		}
	}
	else
	{
		throw;
	}
}

void CPolaCustomBeam::setBeamProperty(const Adesk::Int32 & beam_property)
{
	assertWriteEnabled();
	beam_property_ = beam_property;
}

void CPolaCustomBeam::addVertexAt(const int& index, const AcGePoint3d & vertex)
{
	assertWriteEnabled();
	beam_vertexes_.insertAt(index, vertex);
	vertexes_num_++;
	if (index > 0)
	{
		UpdateOffsetLine(0.5 * beam_b_);
	}
}

void CPolaCustomBeam::addViewableAt(const int index, const Adesk::Int32 viewable)
{
	assertWriteEnabled();
	if (viewable == 0 || viewable == 1)
	{
		if (index == 0)
		{
			beam_viewable_.push_back(viewable);
		}
		else
		{
			auto iterators = beam_viewable_.begin() + index;
			beam_viewable_.insert(iterators, viewable);
		}
	}
	else
	{
		throw;
	}
}

void CPolaCustomBeam::UpdateOffsetLine(const double& distance)
{
	assertWriteEnabled();
	top_offset_vertex_.removeAll();
	bottom_offset_vertex_.removeAll();
	AcGePoint3dArray temp_top, temp_bottom;
	BasicTools::OffsetPolyLine(beam_vertexes_, distance, temp_top);
	BasicTools::OffsetPolyLine(beam_vertexes_, -distance, temp_bottom);
	for (int i = 0; i < temp_top.length(); i++)
	{
		top_offset_vertex_.append(temp_top.at(i));
		bottom_offset_vertex_.append(temp_bottom.at(i));
	}
}

AcDbObjectId CPolaCustomBeam::DrawBeamWithOffset(CPolaCustomBeam * beam, const double offset_distance)
{
	int index = 2;
	TCHAR keyword[256] = { 0 };
	beam->addViewableAt(0, 0);
	AcGePoint3d start_point;

	if (!SelectEntitys::PickPoint(_T("Pick first point:\n"), start_point))
		return AcDbObjectId::kNull;
	AcGePoint3d previous_point = start_point;
	AcGePoint3d current_point;
	AcDbObjectId beam_id = AcDbObjectId::kNull;

	while (SelectEntitys::PickPoint(_T("Pick next point:\n"), start_point, current_point))
	{
		if (index == 2)
		{
			if (offset_distance != 0.0)
			{
				AcGePoint3d temp_point[2];
				BasicTools::OffsetLineSegment(start_point, current_point, offset_distance, temp_point);
				beam->addVertexAt(0, temp_point[0]);
				beam->addVertexAt(1, temp_point[1]);
			}
			else
			{
				beam->addVertexAt(0, previous_point);
				beam->addVertexAt(1, current_point);
			}

			InputValue::GetKeyword(
				_T("Please enter the visibility of the beam segment: [Visible/Invisible]"),
				_T("Visible Invisible"), keyword, sizeof(keyword) / sizeof(TCHAR));

			if (_tcscmp(keyword, _T("Visible")) == 0)
				beam->addViewableAt(index - 1, 1);
			else if (_tcscmp(keyword, _T("Invisible")) == 0)
				beam->addViewableAt(index - 1, 0);
			else
				throw;

			beam_id = AddToModelSpace::AddEntityToModelSpace(beam);
		}
		else if (index > 2)
		{
			CPolaCustomBeam* beam = nullptr;
			if (acdbOpenObject(beam, beam_id, AcDb::kForWrite) == Acad::eOk)
			{
				if (offset_distance != 0.0)
				{
					AcGePoint3d temp_point[2];
					BasicTools::OffsetLineSegment(previous_point, current_point, offset_distance, temp_point);
					beam->addVertexAt(index - 1, temp_point[1]);
				}
				else
				{
					beam->addVertexAt(index - 1, current_point);
				}

				InputValue::GetKeyword(
					_T("Please enter the visibility of the beam segment: [Visible/Invisible]"),
					_T("Visible Invisible"), keyword, sizeof(keyword) / sizeof(TCHAR));

				if (_tcscmp(keyword, _T("Visible")) == 0)
					beam->addViewableAt(index - 1, 1);
				else if (_tcscmp(keyword, _T("Invisible")) == 0)
					beam->addViewableAt(index - 1, 0);
				else
					throw;

				beam->close();
			}
		}
		beam->recordGraphicsModified();
		acedUpdateDisplay();

		previous_point = current_point;
		index++;
	}
	return beam_id;
}

AcDbObjectId CPolaCustomBeam::PickCenterPointDrawBeam(CPolaCustomBeam * beam)
{
	return DrawBeamWithOffset(beam, 0);
}

AcDbObjectId CPolaCustomBeam::PickTopPointDrawBeam(CPolaCustomBeam * beam)
{
	return DrawBeamWithOffset(beam, beam->getBeamWidth() * 0.5);
}

AcDbObjectId CPolaCustomBeam::PickBottomPointDrawBeam(CPolaCustomBeam * beam)
{
	return DrawBeamWithOffset(beam, -beam->getBeamWidth() * 0.5);
}

Acad::ErrorStatus CPolaCustomBeam::subTransformBy(const AcGeMatrix3d & xfrom)
{
	assertReadEnabled();
	for (int i = 0;i < vertexes_num_;i++)
	{
		beam_vertexes_.at(i).transformBy(xfrom);
		top_offset_vertex_.at(i).transformBy(xfrom);
		bottom_offset_vertex_.at(i).transformBy(xfrom);
	}
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomBeam::subGetGeomExtents(AcDbExtents & extents) const
{
	assertReadEnabled();
	extents = AcDbExtents();
	const auto addPointsToExtents = [&extents](const AcGePoint3dArray& vertices)
		{
			for (const auto& vertex : vertices)
			{
				extents.addPoint(vertex);
			}
		};

	addPointsToExtents(beam_vertexes_);
	addPointsToExtents(top_offset_vertex_);
	addPointsToExtents(bottom_offset_vertex_);
	return Acad::eOk;
}

AcDbObjectIdArray CPolaCustomBeam::GetIntersectingPillar() const
{
	AcDbObjectIdArray intersecting_pillar_ids;

	AcDbExtents beam_extents;					// get beam extents
	if (getGeomExtents(beam_extents) != Acad::eOk)
	{
		acutPrintf(_T("Can't get beam extents!\n"));
		return intersecting_pillar_ids;
	}

	AcDbObjectIdArray pillar_ids_all;			// get all pillar ids in database
	if (!SelectEntitys::GetAllEntitysByType(CPolaCustomPillar::desc(), pillar_ids_all))
	{
		acutPrintf(_T("Cant't get pillar list!\n"));
		return intersecting_pillar_ids;
	}

	AcDbObjectPointer<AcDbEntity> pillar_entity;		// check weather intersect  [new] smart pointer!
	AcDbExtents pillar_extents;

	for (AcDbObjectId pillar_id : pillar_ids_all)
	{
		if (pillar_entity.open(pillar_id, AcDb::kForRead) != Acad::eOk)
		{
			acutPrintf(_T("Can't open pillar entity: %lu\n"), pillar_id.handle());
			continue;
		}

		if (pillar_entity->getGeomExtents(pillar_extents) != Acad::eOk)
		{
			acutPrintf(_T("Pillar(ID: %lu) get extents failure\n"), pillar_id.handle());
			continue;
		}

		AcDbBlockReference* block_reference = AcDbBlockReference::cast(pillar_entity);
		if (block_reference)
		{
			pillar_extents.transformBy(block_reference->blockTransform());
		}

		if (BasicTools::IsIntersectRectangle(beam_extents, pillar_extents))
		{
			intersecting_pillar_ids.append(pillar_id);
		}
	}
	return intersecting_pillar_ids;
}

void CPolaCustomBeam::addJoint(const double slab_thickness)
{
	AcDbObjectIdArray intersecting_pillar_ids = GetIntersectingPillar();
	if (vertexes_num_ < 2)
	{
		acutPrintf(_T("Beam vertexes number less than 2, can't add joint.\n"));
		return;
	}
	for (int i = 1;i < beam_vertexes_.length() - 1;i++)
	{
		if (beam_viewable_.at(i) == beam_viewable_.at(i + 1))
			continue;
		for (const auto& pillar_id : intersecting_pillar_ids)
		{
			AcDbEntity* pillar_entity = nullptr;
			if (acdbOpenObject(pillar_entity, pillar_id, OpenMode::kForRead) != Acad::eOk)
				continue;
			CPolaCustomPillar* pillar = CPolaCustomPillar::cast(pillar_entity);
			const AcGePoint3d& temp_pillar_center_point = pillar->getCenterPoint();

			if (temp_pillar_center_point != beam_vertexes_.at(i))
			{
				pillar_entity->close();
				continue;
			}
			AcDbLine* joint_line1 = new AcDbLine();
			AcDbLine* joint_line2 = new AcDbLine();
			AcDbLine* joint_line3 = new AcDbLine();
			AcDbLine* joint_line4 = new AcDbLine();
			double pillar_b, pillar_h;
			pillar->getDiameter(pillar_b, pillar_h);

			joint_line1->setStartPoint(AcGePoint3d(temp_pillar_center_point.x - pillar_b / 2.0 - 100 - beam_h_ + slab_thickness, temp_pillar_center_point.y + beam_b_ / 2, 0));
			joint_line1->setEndPoint(AcGePoint3d(temp_pillar_center_point.x - pillar_b / 2.0 - 100 - beam_h_ + slab_thickness, temp_pillar_center_point.y - beam_b_ / 2, 0));

			joint_line2->setStartPoint(AcGePoint3d(temp_pillar_center_point.x - pillar_b / 2.0 - 100, temp_pillar_center_point.y + beam_b_ / 2, 0));
			joint_line2->setEndPoint(AcGePoint3d(temp_pillar_center_point.x - pillar_b / 2.0 - 100, temp_pillar_center_point.y - beam_b_ / 2, 0));

			joint_line3->setStartPoint(AcGePoint3d(temp_pillar_center_point.x + pillar_b / 2.0 + 100, temp_pillar_center_point.y + beam_b_ / 2, 0));
			joint_line3->setEndPoint(AcGePoint3d(temp_pillar_center_point.x + pillar_b / 2.0 + 100, temp_pillar_center_point.y - beam_b_ / 2, 0));

			joint_line4->setStartPoint(AcGePoint3d(temp_pillar_center_point.x + pillar_b / 2.0 + 100 + beam_h_ - slab_thickness, temp_pillar_center_point.y + beam_b_ / 2, 0));
			joint_line4->setEndPoint(AcGePoint3d(temp_pillar_center_point.x + pillar_b / 2.0 + 100 + beam_h_ - slab_thickness, temp_pillar_center_point.y - beam_b_ / 2, 0));
			if (beam_viewable_.at(i) == 1)
			{
				joint_line1->setLinetype(StyleTools::GetLineStyleId(_T("DASHED")));
				joint_line1->setLinetypeScale(700);
				joint_line2->setLinetype(StyleTools::GetLineStyleId(_T("DASHED")));
				joint_line2->setLinetypeScale(700);
				joint_line3->setLinetype(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
				joint_line4->setLinetype(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
				AcDbLine* temp_line1 = new AcDbLine(joint_line2->startPoint(), joint_line4->startPoint());
				AcDbLine* temp_line2 = new AcDbLine(joint_line2->endPoint(), joint_line4->endPoint());
				AddToModelSpace::AddEntityToModelSpace(joint_line1);
				AddToModelSpace::AddEntityToModelSpace(joint_line2);
				AddToModelSpace::AddEntityToModelSpace(joint_line3);
				AddToModelSpace::AddEntityToModelSpace(joint_line4);
				AddToModelSpace::AddEntityToModelSpace(temp_line1);
				AddToModelSpace::AddEntityToModelSpace(temp_line2);
			}
			else
			{
				joint_line3->setLinetype(StyleTools::GetLineStyleId(_T("DASHED")));
				joint_line3->setLinetypeScale(700);
				joint_line4->setLinetype(StyleTools::GetLineStyleId(_T("DASHED")));
				joint_line4->setLinetypeScale(700);
				joint_line1->setLinetype(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
				joint_line2->setLinetype(StyleTools::GetLineStyleId(_T("CONTINUOUS")));

				AcDbLine* temp_line1 = new AcDbLine(joint_line1->startPoint(), joint_line3->startPoint());
				AcDbLine* temp_line2 = new AcDbLine(joint_line1->endPoint(), joint_line3->endPoint());

				AddToModelSpace::AddEntityToModelSpace(joint_line1);
				AddToModelSpace::AddEntityToModelSpace(joint_line2);
				AddToModelSpace::AddEntityToModelSpace(joint_line3);
				AddToModelSpace::AddEntityToModelSpace(joint_line4);
				AddToModelSpace::AddEntityToModelSpace(temp_line1);
				AddToModelSpace::AddEntityToModelSpace(temp_line2);
			}
		}
	}
}

AcDbObjectId CPolaCustomBeam::addBeamSnInfo()
{
	if (beam_vertexes_.isEmpty())
		return AcDbObjectId::kNull;
	AcGePoint3d insert_point = AcGePoint3d(getHorizontalMidPoint().x - 3000, getHorizontalMidPoint().y + beam_b_ / 2 + 500, 0);
	std::wstring info;
	std::wstringstream info_stream;

	info_stream << _T("KL") << beam_property_ << _T(" ") << static_cast<int>(beam_b_) << _T("*") << static_cast<int>(beam_h_);
	info = info_stream.str();

	return DrawEntity::AddText(insert_point, info.c_str(), StyleTools::InitTextStyle(), 450);
}

