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
	for (int i = 0; i < vertexes_num_; i++)
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
	for (int i = 0; i < vertexes_num_ - 1; i++)
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
	for (int i = 0; i < vertexes_num_ - 1; i++)
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
	
	UpdateOffsetLine();

	mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CENTER")));
	mode->subEntityTraits().setLineTypeScale(70);
	mode->subEntityTraits().setColor(8);

	mode->geometry().polyline(vertexes_num_, beam_vertexes_.asArrayPtr());			// center line.

	mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("CONTINUOUS")));
	mode->subEntityTraits().setColor(2);
	mode->geometry().polyline(vertexes_num_, top_offset_vertex_.asArrayPtr());		// top offset line
	mode->geometry().polyline(vertexes_num_, bottom_offset_vertex_.asArrayPtr());	// bottom offset line

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
	Acad::ErrorStatus error_status;
	AcDbPolyline* poly_line = new AcDbPolyline();
	for (int i = 0;i < vertexes_num_;i++)
	{
		poly_line->addVertexAt(i, BasicTools::Point3dToPoint2d(beam_vertexes_.at(i)));
	}
	error_status = poly_line->getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
	return error_status;
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
	Acad::ErrorStatus error_status;
	AcDbPolyline* poly_line = new AcDbPolyline();
	for (int i = 0;i < vertexes_num_;i++)
	{
		poly_line->addVertexAt(i, BasicTools::Point3dToPoint2d(beam_vertexes_.at(i)));
	}
	error_status = poly_line->getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
	return error_status;
}

//- Grip points protocol
Acad::ErrorStatus CPolaCustomBeam::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints(gripPoints, osnapModes, geomIds));
//	for (int i = 0;i < vertexes_num_;i++)
//	{
//		gripPoints.append(beam_vertexes_.at(i));
//	}
//	return Acad::eOk;
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
	beam_vertexes_.insertAt(index, vertex);
	vertexes_num_++;
	if (index > 2)
	{
		UpdateOffsetLine();
	}
}

void CPolaCustomBeam::UpdateOffsetLine()
{
	top_offset_vertex_.removeAll();
	bottom_offset_vertex_.removeAll();
	BasicTools::OffsetPolyLine(beam_vertexes_, 0.5 * beam_b_, top_offset_vertex_);
	BasicTools::OffsetPolyLine(beam_vertexes_, -0.5 * beam_b_, bottom_offset_vertex_);
}
