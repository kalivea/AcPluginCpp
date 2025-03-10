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

//
#include "PillarTools.h"
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
			throw;
		}
	}
	const bool isClosed = IsBeamClosed();
	if (!isClosed)
	{
		mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
		AcDbLine left_side_line(top_offset_vertex_.first(), bottom_offset_vertex_.first());
		AcDbLine right_side_line(top_offset_vertex_.last(), bottom_offset_vertex_.last());
		left_side_line.worldDraw(mode);
		right_side_line.worldDraw(mode);
	}

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
	delete subordinate_line_segment;
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

Acad::ErrorStatus CPolaCustomBeam::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset) {
	assertWriteEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	//return (AcDbEntity::subMoveGripPointsAt(indices, offset));
	for (int i = 0; i < indices.length(); ++i)
	{
		int index = indices[i];
		if (index >= 0 && index < beam_vertexes_.length())
		{
			beam_vertexes_[index] += offset;
		}
	}
	UpdateOffsetLine(0.5 * beam_b_);
	GenerateBeamSegmentDirection();
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
	std::vector<Adesk::Int32> v = getBeamViewable();
	return v.at(index);
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
	double length = 0;
	for (int i = 0;i < vertexes_num_ - 1;i++)
	{
		length += beam_vertexes_.at(i).distanceTo(beam_vertexes_.at(i + 1));
	}
	return length;
}

bool CPolaCustomBeam::IsBeamClosed() const
{
	return beam_vertexes_.first().isEqualTo(beam_vertexes_.last());
}

AcGeVector3dArray CPolaCustomBeam::getBeamSegmentDirection() const
{
	assertReadEnabled();
	return beam_segment_direction;
}

AcGePoint3d CPolaCustomBeam::getHorizontalMidPoint() const
{
	assertReadEnabled();
	if (beam_vertexes_.length() < 2)
	{
		return AcGePoint3d(6496, 6496, 6496);
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

			isMidOnHorizontal = (fabs(start.y - end.y) < AcGeContext::gTol.equalPoint());
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

		if (fabs(start.y - end.y) < AcGeContext::gTol.equalPoint())
		{
			AcGePoint3d mid_point((start.x + end.x) / 2.0, (start.y + end.y) / 2.0, (start.z + end.z) / 2.0);
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

void CPolaCustomBeam::setBeamSegmentDireciton(const AcGeVector3dArray & segment_direction)
{
	assertWriteEnabled();
	beam_segment_direction = segment_direction;
}

void CPolaCustomBeam::addVertexAt(const int index, const AcGePoint3d & vertex)
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

void CPolaCustomBeam::resetViewableAt(const int index, const Adesk::Int32 viewable)
{
	assertWriteEnabled();
	if (index <= 0 || index >= beam_viewable_.size())
		throw;
	beam_viewable_.at(index) = viewable;
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

void CPolaCustomBeam::GenerateBeamSegmentDirection()
{
	assertWriteEnabled();
	beam_segment_direction.removeAll();
	for (int i = 0;i < beam_vertexes_.length() - 1;i++)
	{
		beam_segment_direction.append(BasicTools::GetVectorBetweenTwoPoint(beam_vertexes_.at(i), beam_vertexes_.at(i + 1)));
	}
}

AcDbObjectId CPolaCustomBeam::DrawBeamWithOffset(CPolaCustomBeam * beam, const double offset_distance)
{
	int index = 2;
	TCHAR keyword[256] = { 0 };
	beam->addViewableAt(0, 0);
	AcGePoint3d start_point;

	bool first_point_selected = false;
	while (!first_point_selected)
	{
		if (SelectEntitys::PickPoint(_T("Pick first point (required):\n"), start_point))
		{
			first_point_selected = true;
		}
		else
		{
			acutPrintf(_T("First point selection was canceled. This point is required. Please try again.\n"));
		}
	}

	AcGePoint3d previous_point = start_point;
	AcGePoint3d current_point;
	AcDbObjectId beam_id = AcDbObjectId::kNull;

	bool second_point_selected = false;
	while (!second_point_selected)
	{
		if (SelectEntitys::PickPoint(_T("Pick next point (required to create a beam):\n"), start_point, current_point))
		{
			second_point_selected = true;
		}
		else
		{
			acutPrintf(_T("Second point selection was canceled. At least two points are required. Please try again.\n"));
		}
	}

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

	while (true)
	{
		InputValue::GetKeyword(
			_T("Please enter the visibility of the beam segment: [Visible/Invisible]"),
			_T("Visible Invisible"), keyword, sizeof(keyword) / sizeof(TCHAR));

		if (_tcscmp(keyword, _T("Visible")) == 0)
		{
			beam->addViewableAt(index - 1, 1);
			break;
		}
		else if (_tcscmp(keyword, _T("Invisible")) == 0)
		{
			beam->addViewableAt(index - 1, 0);
			break;
		}
		else
		{
			acutPrintf(_T("Invalid input. Please enter 'Visible' or 'Invisible'.\n"));
		}
	}

	beam_id = AddToModelSpace::AddEntityToModelSpace(beam);

	previous_point = current_point;
	index++;

	while (true)
	{
		if (!SelectEntitys::PickPoint(_T("Pick next point (required):\n"), start_point, current_point))
		{
			acutPrintf(_T("Beam creation completed with %d points.\n"), index - 1);
			break;
		}

		CPolaCustomBeam* beam_ptr = nullptr;
		if (acdbOpenObject(beam_ptr, beam_id, AcDb::kForWrite) == Acad::eOk)
		{
			if (offset_distance != 0.0)
			{
				AcGePoint3d temp_point[2];
				BasicTools::OffsetLineSegment(previous_point, current_point, offset_distance, temp_point);
				beam_ptr->addVertexAt(index - 1, temp_point[1]);
			}
			else
			{
				beam_ptr->addVertexAt(index - 1, current_point);
			}

			while (true)
			{
				InputValue::GetKeyword(
					_T("Please enter the visibility of the beam segment: [Visible/Invisible]"),
					_T("Visible Invisible"), keyword, sizeof(keyword) / sizeof(TCHAR));

				if (_tcscmp(keyword, _T("Visible")) == 0)
				{
					beam_ptr->addViewableAt(index - 1, 1);
					break;
				}
				else if (_tcscmp(keyword, _T("Invisible")) == 0)
				{
					beam_ptr->addViewableAt(index - 1, 0);
					break;
				}
				else
				{
					acutPrintf(_T("Invalid input. Please enter 'Visible' or 'Invisible'.\n"));
				}
			}
			beam_ptr->close();
		}

		beam->recordGraphicsModified();
		acedUpdateDisplay();

		previous_point = current_point;
		index++;
	}

	beam->close();
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

bool CPolaCustomBeam::ModifyViewable(CPolaCustomBeam * beam, int index, Adesk::Int32 viewable)
{
	if (index <= 0 || index >= beam->getVertexesNum())
	{
		acutPrintf(_T("Invalid index!\n"));
		return false;
	}
	else
	{
		beam->resetViewableAt(index, viewable);
		beam->recordGraphicsModified();
		acedUpdateDisplay();
		beam->close();
		return true;
	}
}

bool CPolaCustomBeam::ModifyViewable(AcDbObjectId beam_id, int index, Adesk::Int32 viewable)
{
	AcDbEntity* beam_entity = nullptr;
	acdbOpenObject(beam_entity, beam_id, OpenMode::kForWrite);
	CPolaCustomBeam* beam = CPolaCustomBeam::cast(beam_entity);
	if (ModifyViewable(beam, index, viewable))
	{
		beam->close();
		return true;
	}
	else
	{
		beam->close();
		return false;
	}
}

bool CPolaCustomBeam::ModifyViewable(CPolaCustomBeam * beam, int index)
{
	AcDbObjectPointer<CPolaCustomBeam> beam_t;
	beam_t.acquire(beam);
	return ModifyViewable(beam_t, index, beam_t->getViewableAt(index) ^ 1);
}

AcDbObjectId CPolaCustomBeam::SelectPillarDrawBeam(CPolaCustomBeam * beam)
{
	AcDbObjectIdArray selected_pillar;
	AcGePoint3dArray vertex_array;
	AcGePoint3dArray sort_vertex_array;
	std::vector<Adesk::Int32> viewable;
	viewable.push_back(1);

	if (SelectEntitys::PickEntitys(_T("Select pillar\n"), CPolaCustomPillar::desc(), selected_pillar))
	{
		for (const auto& pillar : selected_pillar)
		{
			AcDbEntity* current_pillar_entity = nullptr;
			acdbOpenObject(current_pillar_entity, pillar, OpenMode::kForRead);
			CPolaCustomPillar* current_pillar = CPolaCustomPillar::cast(current_pillar_entity);
			vertex_array.append(current_pillar->getCenterPoint());
			viewable.push_back(1);
			current_pillar->close();
			current_pillar_entity->close();
		}
		BasicTools::SortPointFromLeftToRight(vertex_array, sort_vertex_array);
		beam->setBeamVertexes(sort_vertex_array);
		beam->setBeamViewable(viewable);
		beam->UpdateOffsetLine(0.5 * beam->getBeamWidth());
		return AddToModelSpace::AddEntityToModelSpace(beam);
	}
	return AcDbObjectId::kNull;
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

Adesk::Int32 CPolaCustomBeam::GetSegmentIndexByYProjection(const AcGePoint3d & point, const AcGeTol & tol) const
{
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
	if (beam_vertexes_.length() < 2)
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

AcDbObjectIdArray CPolaCustomBeam::addJoint(const double slab_thickness, const double offset_length)
{
	AcDbObjectIdArray joint_line_ids;
	if (vertexes_num_ < 2)
	{
		acutPrintf(_T("Beam vertexes number less than 2, can't add joint.\n"));
		return joint_line_ids;
	}

	AcDbObjectIdArray intersecting_pillar_ids = GetIntersectingPillar();
	if (intersecting_pillar_ids.isEmpty())
	{
		return joint_line_ids;
	}

	auto CreateLine = [&joint_line_ids](const AcGePoint3d& start, const AcGePoint3d& end, const ACHAR* line_type, double scale_if_dashed = 1.0)
		{
			AcDbLine* line = new AcDbLine(start, end);
			line->setLinetype(StyleTools::GetLineStyleId(line_type));
			if (_tcscmp(line_type, _T("DASHED")) == 0)
			{
				line->setLinetypeScale(scale_if_dashed);
			}
			joint_line_ids.append(AddToModelSpace::AddEntityToModelSpace(line));
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
			if (acdbOpenObject(pillar_entity, pillar_id, OpenMode::kForRead) != Acad::eOk)
			{
				continue;
			}

			std::unique_ptr<AcDbEntity, std::function<void(AcDbEntity*)>> pillar_guard(
				pillar_entity, [](AcDbEntity* p) { if (p) p->close(); });

			CPolaCustomPillar* pillar = CPolaCustomPillar::cast(pillar_entity);
			if (!pillar || pillar->getCenterPoint() != vertex)
			{
				continue;
			}

			double pillar_b, pillar_h;
			pillar->getDiameter(pillar_b, pillar_h);

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
			break;
		}
	}
	return joint_line_ids;
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

Acad::ErrorStatus CPolaCustomBeam::InsertVertex(const AcGePoint3d & insert_point)
{
	assertWriteEnabled();
	int insert_index = -1;
	//insert_index = GetSegmentIndexByYProjection(insert_point);
	insert_index = GetSegmentIndex(insert_point);
	if (insert_index < 0)
		return Acad::eInvalidInput;
	addVertexAt(insert_index + 1, insert_point);
	addViewableAt(insert_index + 1, 1);
	UpdateOffsetLine(0.5 * beam_b_);
	this->recordGraphicsModified();
	acedUpdateDisplay();
	return Acad::eOk;
}
