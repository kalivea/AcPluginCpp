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

CPolaCustomPillar::CPolaCustomPillar(const CPolaCustomPillar & pillar_template)
	:center_point_(pillar_template.center_point_), direction_vector_(pillar_template.direction_vector_), pillar_d_(pillar_template.pillar_d_),
	pillar_h_(pillar_template.pillar_h_), viewable_(pillar_template.viewable_), pillar_property_(pillar_template.pillar_property_),
	pillar_serial_number_(pillar_template.pillar_serial_number_), pillar_type_(pillar_template.pillar_type_)
{
	CalculateVertex();
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

	es = pFiler->writeItem(pillar_type_);
	if (es != Acad::eOk)
		return es;
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
	if (pillar_type_ == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			es = pFiler->writeItem(vertex_.at(i));
			if (es != Acad::eOk)
				return es;
		}
	}
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
	es = pFiler->readItem(&pillar_type_);
	if (es != Acad::eOk)
		return es;
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
	if (pillar_type_ == 1)
	{
		vertex_.removeAll();
		CalculateVertex();
		for (int i = 0; i < 4; i++)
		{
			es = pFiler->readItem(&vertex_.at(i));
			if (es != Acad::eOk)
				return es;
		}
	}
	return (pFiler->filerStatus());
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaCustomPillar::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	if (pillar_type_ == 0)
	{
		if (viewable_)
		{
			mode->geometry().circle(center_point_, pillar_d_ * 0.5, AcGeVector3d::kZAxis);
		}
		else
		{
			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
			mode->geometry().circle(center_point_, pillar_d_ * 0.5, AcGeVector3d::kZAxis);
		}
	}
	else if (pillar_type_ == 1)
	{
		if (!vertex_.isEmpty())
		{
			if (viewable_)
			{
				AcDbPolyline* pl = new AcDbPolyline();
				for (int i = 0; i < vertex_.length(); i++)
				{
					pl->addVertexAt(i, BasicTools::Point3dToPoint2d(vertex_.at(i)));
				}
				pl->setDatabaseDefaults();
				pl->setClosed(true);
				pl->worldDraw(mode);
				delete pl;

				mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
				mode->subEntityTraits().setLineTypeScale(100);
				AcDbLine(vertex_.at(0), vertex_.at(2)).worldDraw(mode);
				AcDbLine(vertex_.at(1), vertex_.at(3)).worldDraw(mode);
			}
			else
			{
				mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
				mode->subEntityTraits().setLineTypeScale(100);

				AcDbPolyline* pl = new AcDbPolyline();
				for (int i = 0; i < vertex_.length(); i++)
				{
					pl->addVertexAt(i, BasicTools::Point3dToPoint2d(vertex_.at(i)));
				}
				pl->setDatabaseDefaults();
				pl->setClosed(true);
				pl->worldDraw(mode);
				delete pl;

				AcDbLine(vertex_.at(0), vertex_.at(2)).worldDraw(mode);
				AcDbLine(vertex_.at(1), vertex_.at(3)).worldDraw(mode);
			}
		}
		else
		{
			throw;
		}
	}
	else
	{
		throw;
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
	Acad::ErrorStatus error_status;
	if (pillar_type_ == 0)
	{
		AcDbCircle* contour_circle = new AcDbCircle();
		contour_circle->setCenter(center_point_);
		contour_circle->setRadius(pillar_d_ * 0.5);

		error_status = contour_circle->getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
		delete contour_circle;
	}
	else if (pillar_type_ == 1)
	{
		AcDbPolyline* contour_polyline = new AcDbPolyline();
		for (int i = 0; i < vertex_.length(); i++)
		{
			contour_polyline->addVertexAt(i, BasicTools::Point3dToPoint2d(vertex_.at(i)));
		}
		contour_polyline->setClosed(true);
		error_status = contour_polyline->getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
		delete contour_polyline;
	}
	else
	{
		throw;
	}
	return error_status;
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
	Acad::ErrorStatus error_status;
	if (pillar_type_ == 0)
	{
		AcDbCircle* contour_circle = new AcDbCircle();
		contour_circle->setCenter(center_point_);
		contour_circle->setRadius(pillar_d_ * 0.5);
		error_status = contour_circle->getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
		delete contour_circle;
	}
	else if (pillar_type_ == 1)
	{
		AcDbPolyline* contour_polyline = new AcDbPolyline();
		for (int i = 0; i < vertex_.length(); i++)
		{
			contour_polyline->addVertexAt(i, BasicTools::Point3dToPoint2d(vertex_.at(i)));
		}
		contour_polyline->setClosed(true);
		error_status = contour_polyline->getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
		delete contour_polyline;
	}
	else
	{
		throw;
	}
	return error_status;
}

//- Grip points protocol
Acad::ErrorStatus CPolaCustomPillar::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)
	if (pillar_type_ == 0)
	{
		gripPoints.append(center_point_);
	}
	else if (pillar_type_ == 1)
	{
		gripPoints.append(center_point_);
		for (int i = 0; i < vertex_.length(); i++)
		{
			gripPoints.append(vertex_.at(i));
		}
	}
	else
	{
		throw;
	}
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomPillar::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset) {
	assertWriteEnabled();
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	// do nothing when move grip points          -----------Pola

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

void CPolaCustomPillar::setCenterPoint(const AcGePoint3d & center)
{
	assertWriteEnabled();
	center_point_ = center;
}

void CPolaCustomPillar::setDirectionVector(const AcGeVector3d & dir_vec)
{
	assertWriteEnabled();
	direction_vector_ = dir_vec;
}

void CPolaCustomPillar::setDiameter(const double& d, const double& h)
{
	assertWriteEnabled();
	pillar_d_ = d;
	pillar_h_ = h;
}

void CPolaCustomPillar::setViewable(const bool view)
{
	assertWriteEnabled();
	viewable_ = view;
}

void CPolaCustomPillar::setPillarProperty(const Adesk::Int32 & prop)
{
	assertWriteEnabled();
	if (prop == 0)
		pillar_property_ = prop;
	else if (prop == 1)
		pillar_property_ = prop;
	else
		throw;
}

void CPolaCustomPillar::setSn(const Adesk::Int32 & sn)
{
	assertWriteEnabled();
	pillar_serial_number_ = sn;
}

void CPolaCustomPillar::setPillarType(const Adesk::Int32 & type)
{
	assertWriteEnabled();
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
	assertWriteEnabled();
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

Acad::ErrorStatus CPolaCustomPillar::subTransformBy(const AcGeMatrix3d & transform_matrix)
{
	if (getPillarType() == 0)
	{
		center_point_.transformBy(transform_matrix);
	}
	else if (getPillarType() == 1)
	{
		for (int i = 0; i < vertex_.length(); i++)
		{
			vertex_.at(i).transformBy(transform_matrix);
		}
		center_point_.transformBy(transform_matrix);
		direction_vector_ = direction_vector_.transformBy(transform_matrix);
	}
	else
	{
		throw;
	}
	return Acad::eOk;
}

bool CPolaCustomPillar::checkValue(const CPolaCustomPillar * pillar)
{
	bool check_result;
	double d, h;
	pillar->getDiameter(d, h);
	bool size_result = (d > 0 && h > 0) ? true : false;
	bool prop_result = (pillar->getPillarProperty() == 0 || pillar->getPillarProperty() == 1) ? true : false;
	bool type_result = (pillar->getPillarType() == 0 || pillar->getPillarType() == 1) ? true : false;
	bool sn_result = pillar->getPillarSn() > 0 ? true : false;
	if (size_result && prop_result && type_result && sn_result)
		check_result = true;
	else
		check_result = false;

	return check_result;
}

void CPolaCustomPillar::BatchInsert(const CPolaCustomPillar & pillar_template, const AcGePoint3dArray & insert_point_array)
{
	if (pillar_template.getPillarType() == 0)
	{
		if (pillar_template.checkValue(&pillar_template))
		{
			for (int i = 0; i < insert_point_array.length(); i++)
			{
				CPolaCustomPillar* pillar = new CPolaCustomPillar(pillar_template);
				pillar->vertex_.removeAll();
				pillar->setCenterPoint(insert_point_array.at(i));
				AddToModelSpace::AddEntityToModelSpace(pillar);
			}
		}
		else
		{
			throw;
		}
	}
	else if (pillar_template.getPillarType() == 1)
	{
		if (!insert_point_array.isEmpty())
		{
			for (int i = 0; i < insert_point_array.length(); i++)
			{
				CPolaCustomPillar* pillar = new CPolaCustomPillar(pillar_template);
				pillar->vertex_.removeAll();
				if (checkValue(pillar))
				{
					pillar->setCenterPoint(insert_point_array.at(i));
					pillar->CalculateVertex();
					AddToModelSpace::AddEntityToModelSpace(pillar);
				}
				else
				{
					throw;
				}

			}
		}
		else
		{
			throw;
		}
	}
	else
	{
		throw;
	}
}

void CPolaCustomPillar::SingleInsert(const CPolaCustomPillar & pillar_template, const AcGePoint3d & insert_point)
{
	AcGePoint3dArray tmp_array;
	tmp_array.append(insert_point);
	BatchInsert(pillar_template, tmp_array);
}

void CPolaCustomPillar::AddPillarLeader(const CPolaCustomPillar * pillar)
{
	AcGePoint3d point_on_leader = AcGePoint3d(pillar->getCenterPoint().x - 900, pillar->getCenterPoint().y - 900, pillar->getCenterPoint().z);
	CString info;
	double b, h;
	pillar->getDiameter(b, h);
	if (pillar->getPillarType() == 1)
	{
		info.Format(_T("Z%d\n%.0f*%.0f"), pillar->getPillarSn(), b, h);
		DrawEntity::AddMLeader(pillar->getCenterPoint(), point_on_leader, point_on_leader, info);
	}
	else if (pillar->getPillarType() == 0)
	{
		info.Format(_T("Z%d\n %%%%c %.0f"), pillar->getPillarSn(), b);
		DrawEntity::AddMLeader(pillar->getCenterPoint(), point_on_leader, point_on_leader, info);
	}
	else
	{
		throw;
	}
}
