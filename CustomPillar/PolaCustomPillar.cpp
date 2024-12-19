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
//----- PolaCustomPillar.cpp : Implementation of CPolaCustomPillar
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PolaCustomPillar.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CPolaCustomPillar::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	CPolaCustomPillar, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, POLACUSTOMPILLAR,
	POLACUSTOMPILLARAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
CPolaCustomPillar::CPolaCustomPillar() : AcDbEntity() {
}

CPolaCustomPillar::~CPolaCustomPillar() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CPolaCustomPillar::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(CPolaCustomPillar::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....
	es = pFiler->writeItem(center_point_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(direction_vector_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(pillar_d_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(pillar_h_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(viewable_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(pillar_property_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(pillar_serial_number_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->writeItem(pillar_type_);
	if (es != Acad::eOk)
		return es;

	return (pFiler->filerStatus());
}

Acad::ErrorStatus CPolaCustomPillar::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > CPolaCustomPillar::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CPolaCustomPillar::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	es = pFiler->readItem(&center_point_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&direction_vector_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&pillar_d_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&pillar_h_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&viewable_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&pillar_property_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&pillar_serial_number_);
	if (es != Acad::eOk)
		return es;
	es = pFiler->readItem(&pillar_type_);
	if (es != Acad::eOk)
		return es;

	return (pFiler->filerStatus());
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaCustomPillar::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	if (pillar_type_ == 0)
	{
		mode->geometry().circle(center_point_, this->pillar_d_ * 0.5, AcGeVector3d::kZAxis);
	}
	else if (pillar_type_ == 1)
	{
		if (!vertex_.isEmpty())
		{
	/*		AcDbPolyline* poly_line = new AcDbPolyline();
			for (int i = 0; i < vertex_.length(); i++)
			{
				poly_line->addVertexAt(i, BasicTools::Point3dToPoint2d(vertex_).at(i));
			}
			poly_line->setClosed(true);
			poly_line->close();
			poly_line->worldDraw(mode);*/
			vertex_.append(vertex_.at(0));
			mode->geometry().polyline(vertex_.length(), vertex_.asArrayPtr());
			vertex_.removeLast();
			AcDbLine(vertex_.at(0), vertex_.at(2)).worldDraw(mode);
			AcDbLine(vertex_.at(1), vertex_.at(3)).worldDraw(mode);
		}
	}
	return (AcDbEntity::subWorldDraw(mode));
}


Adesk::UInt32 CPolaCustomPillar::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

//- Osnap points protocol
Acad::ErrorStatus CPolaCustomPillar::subGetOsnapPoints(
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

Acad::ErrorStatus CPolaCustomPillar::subGetOsnapPoints(
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
Acad::ErrorStatus CPolaCustomPillar::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints(gripPoints, osnapModes, geomIds));
}

Acad::ErrorStatus CPolaCustomPillar::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset) {
	assertWriteEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt(indices, offset));
}

Acad::ErrorStatus CPolaCustomPillar::subGetGripPoints(
	AcDbGripDataPtrArray & grips, const double curViewUnitSize, const int gripSize,
	const AcGeVector3d & curViewDir, const int bitflags
) const {
	assertReadEnabled();

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints(grips, curViewUnitSize, gripSize, curViewDir, bitflags));
}

Acad::ErrorStatus CPolaCustomPillar::subMoveGripPointsAt(
	const AcDbVoidPtrArray & gripAppData, const AcGeVector3d & offset,
	const int bitflags
) {
	assertWriteEnabled();

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt(gripAppData, offset, bitflags));
}
// ---------------------------------------------------------------------------------------------------
// custom pillar define function						----Pola

void CPolaCustomPillar::setCenterPoint(AcGePoint3d center)
{
	assertReadEnabled();
	center_point_ = center;
}

void CPolaCustomPillar::setDirectionVector(AcGeVector3d dir_vec)
{
	assertReadEnabled();
	direction_vector_ = dir_vec;
}

void CPolaCustomPillar::setDiameter(double d, double h)
{
	assertReadEnabled();
	pillar_d_ = d;
	pillar_h_ = h;
}

void CPolaCustomPillar::setViewable(bool view)
{
	assertReadEnabled();
	viewable_ = view;
}

void CPolaCustomPillar::setPillarProperty(Adesk::Int32 prop)
{
	assertReadEnabled();
	if (prop == 0)
		pillar_property_ = prop;
	else if (prop == 1)
		pillar_property_ = prop;
	else
		throw;
}

void CPolaCustomPillar::setSN(Adesk::Int32 sn)
{
	assertReadEnabled();
	pillar_serial_number_ = sn;
}

void CPolaCustomPillar::setPillarType(Adesk::Int32 type)
{
	assertReadEnabled();
	if (type == 0)
		pillar_type_ = type;
	else if (type == 1)
		pillar_type_ = type;
	else
		throw;
}

AcGePoint3d CPolaCustomPillar::getCenterPoint() const
{
	assertReadEnabled();
	return center_point_;
}

AcGeVector3d CPolaCustomPillar::getDirectionVector() const
{
	assertReadEnabled();
	return direction_vector_;
}

void CPolaCustomPillar::getDiameter(double& d, double& h) const
{
	assertReadEnabled();
	d = pillar_d_;
	h = pillar_h_;
}

bool CPolaCustomPillar::getViewable() const
{
	assertReadEnabled();
	return viewable_;
}

Adesk::Int32 CPolaCustomPillar::getPillarProperty() const
{
	assertReadEnabled();
	return pillar_property_;
}

Adesk::Int32 CPolaCustomPillar::getPillarSn() const
{
	assertReadEnabled();
	return pillar_serial_number_;
}

Adesk::Int32 CPolaCustomPillar::getPillarType() const
{
	assertReadEnabled();
	return pillar_type_;
}

void CPolaCustomPillar::CalculateVertex()
{
	assertReadEnabled();
	AcGePoint3d temp_vertex[4];
	if (pillar_type_ == 1)
	{
		if (direction_vector_.angleTo(AcGeVector3d::kXAxis) == 0)
		{
			temp_vertex[0] = AcGePoint3d(center_point_.x - 0.5 * pillar_d_, center_point_.y + 0.5 * pillar_h_, 0);
			temp_vertex[1] = AcGePoint3d(center_point_.x - 0.5 * pillar_d_, center_point_.y - 0.5 * pillar_h_, 0);
			temp_vertex[2] = AcGePoint3d(center_point_.x + 0.5 * pillar_d_, center_point_.y - 0.5 * pillar_h_, 0);
			temp_vertex[3] = AcGePoint3d(center_point_.x + 0.5 * pillar_d_, center_point_.y + 0.5 * pillar_h_, 0);
			for (int i = 0; i < 4; i++)
			{
				vertex_.append(temp_vertex[i]);
			}
		}
		else
		{
			AcGeMatrix3d rotate_transform_matrix;
			rotate_transform_matrix.setToRotation(BasicTools::GetAngleToXaxis(direction_vector_, 1), AcGeVector3d::kZAxis, center_point_);

			temp_vertex[0] = AcGePoint3d(center_point_.x - 0.5 * pillar_d_, center_point_.y + 0.5 * pillar_h_, 0).
				transformBy(rotate_transform_matrix);
			temp_vertex[1] = AcGePoint3d(center_point_.x - 0.5 * pillar_d_, center_point_.y - 0.5 * pillar_h_, 0).
				transformBy(rotate_transform_matrix);
			temp_vertex[2] = AcGePoint3d(center_point_.x + 0.5 * pillar_d_, center_point_.y - 0.5 * pillar_h_, 0).
				transformBy(rotate_transform_matrix);
			temp_vertex[3] = AcGePoint3d(center_point_.x + 0.5 * pillar_d_, center_point_.y + 0.5 * pillar_h_, 0).
				transformBy(rotate_transform_matrix);
			for (int i = 0; i < 4; i++)
			{
				vertex_.append(temp_vertex[i]);
			}
		}
	}
}

void CPolaCustomPillar::UpdateEntity()
{

}
