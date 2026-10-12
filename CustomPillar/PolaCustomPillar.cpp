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
#include <sstream>
#include <tchar.h>
#include <cmath>
#include <memory>
#include <limits>
#include "DrawEntity.h"
#include "../Common/DbObjectOwner.h"
#include "PillarDwgFields.h"

namespace {
bool finitePoint(const AcGePoint3d& value) {
	return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}
bool finiteDirection(const AcGeVector3d& value) {
	return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z)
		&& std::hypot(value.x, value.y) > 0.0 && std::isfinite(value.length()) && value.length() > 0.0;
}
Acad::ErrorStatus buildPillarContour(const AcGePoint3dArray& vertices, AcDbPolyline& polyline) {
	if (vertices.length() != 4) return Acad::eInvalidInput;
	for (int i = 0; i < vertices.length(); ++i) {
		if (!finitePoint(vertices[i])) return Acad::eInvalidInput;
		const Acad::ErrorStatus status = polyline.addVertexAt(i, AcGePoint2d(vertices[i].x, vertices[i].y));
		if (status != Acad::eOk) return status;
	}
	polyline.setClosed(true);
	polyline.setElevation(vertices[0].z);
	return Acad::eOk;
}
const size_t kMaxSerialLength = 65536;
class PillarReadAdapter {
	AcDbDwgFiler* filer_;
public:
	explicit PillarReadAdapter(AcDbDwgFiler* filer) : filer_(filer) {}
	Acad::ErrorStatus readUInt32(Adesk::UInt32* version) { return filer_->readUInt32(version); }
	template<class T> Acad::ErrorStatus readItem(T* value) { return filer_->readItem(value); }
	Acad::ErrorStatus filerStatus() const { return filer_->filerStatus(); }
	Acad::ErrorStatus readString(std::wstring& value) {
		AcString serial;
		const Acad::ErrorStatus status = filer_->readString(serial);
		if (status != Acad::eOk) return status;
		if (static_cast<size_t>(serial.length()) > kMaxSerialLength) return Acad::eInvalidInput;
		value = serial.kwszPtr();
		return Acad::eOk;
	}
};
}
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
CPolaCustomPillar::CPolaCustomPillar() : AcDbEntity(), center_point_(AcGePoint3d::kOrigin),
	direction_vector_(AcGeVector3d::kXAxis), pillar_d_(1.0), pillar_h_(1.0),
	viewable_(true), pillar_property_(0), pillar_type_(0), concrete_grade_(0) {
}

CPolaCustomPillar::CPolaCustomPillar(const CPolaCustomPillar & pillar_template)
	:center_point_(pillar_template.center_point_), direction_vector_(pillar_template.direction_vector_), pillar_d_(pillar_template.pillar_d_),
	pillar_h_(pillar_template.pillar_h_), viewable_(pillar_template.viewable_), pillar_property_(pillar_template.pillar_property_),
	pillar_serial_number_(pillar_template.pillar_serial_number_), pillar_type_(pillar_template.pillar_type_), concrete_grade_(pillar_template.concrete_grade_)
{
	CalculateVertex();
}

CPolaCustomPillar::~CPolaCustomPillar() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CPolaCustomPillar::dwgOutFields(AcDbDwgFiler* pFiler) const {
	assertReadEnabled();
	if (!pFiler) return Acad::eInvalidInput;
	Pola::PillarDwgFields fields;
	fields.type = pillar_type_;
	fields.center = center_point_;
	fields.direction = direction_vector_;
	fields.diameter = pillar_d_;
	fields.height = pillar_h_;
	fields.visible = viewable_;
	fields.property = pillar_property_;
	fields.serial = pillar_serial_number_;
	fields.vertices = rect_pillar_vertex_;
	fields.grade = concrete_grade_;
	Acad::ErrorStatus status = Pola::ValidatePillarFields(fields);
	if (status != Acad::eOk) return status;
	status = AcDbEntity::dwgOutFields(pFiler);
	return status == Acad::eOk ? Pola::WritePillarFields(pFiler, fields) : status;
}

Acad::ErrorStatus CPolaCustomPillar::dwgInFields(AcDbDwgFiler* pFiler) {
	assertWriteEnabled();
	if (!pFiler) return Acad::eInvalidInput;
	Acad::ErrorStatus status = AcDbEntity::dwgInFields(pFiler);
	if (status != Acad::eOk) return status;
	PillarReadAdapter reader(pFiler);
	Pola::PillarDwgFields fields;
	status = Pola::ReadPillarFields(&reader, fields);
	if (status != Acad::eOk) return status;
	center_point_ = fields.center;
	direction_vector_ = fields.direction;
	pillar_d_ = fields.diameter;
	pillar_h_ = fields.height;
	viewable_ = fields.visible;
	pillar_property_ = fields.property;
	pillar_serial_number_.swap(fields.serial);
	pillar_type_ = fields.type;
	concrete_grade_ = fields.grade;
	rect_pillar_vertex_ = fields.vertices;
	return Acad::eOk;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaCustomPillar::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();
	if (!mode || !checkValue(this) || (pillar_type_ == 1 && rect_pillar_vertex_.length() != 4))
		return Adesk::kFalse;
	if (pillar_type_ == 0)
	{
		if (viewable_)
		{
			mode->geometry().circle(center_point_, pillar_d_ * 0.5, AcGeVector3d::kZAxis);
			mode->subEntityTraits().setColor(256);
		}
		else
		{
			mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
			mode->subEntityTraits().setLineTypeScale(700);
			mode->subEntityTraits().setColor(256);
			mode->geometry().circle(center_point_, pillar_d_ * 0.5, AcGeVector3d::kZAxis);
		}
	}
	else if (pillar_type_ == 1)
	{
		if (!rect_pillar_vertex_.isEmpty())
		{
			if (viewable_)
			{
				AcDbPolyline pl;
				if (buildPillarContour(rect_pillar_vertex_, pl) != Acad::eOk) return Adesk::kFalse;
				pl.setDatabaseDefaults();
				mode->subEntityTraits().setColor(256);
				pl.worldDraw(mode);

				mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
				mode->subEntityTraits().setLineTypeScale(300);
				mode->subEntityTraits().setColor(3);
				AcDbLine(rect_pillar_vertex_.at(0), rect_pillar_vertex_.at(2)).worldDraw(mode);
				AcDbLine(rect_pillar_vertex_.at(1), rect_pillar_vertex_.at(3)).worldDraw(mode);
			}
			else
			{
				mode->subEntityTraits().setLineType(StyleTools::GetLineStyleId(_T("DASHED")));
				mode->subEntityTraits().setLineTypeScale(300);

				AcDbPolyline pl;
				if (buildPillarContour(rect_pillar_vertex_, pl) != Acad::eOk) return Adesk::kFalse;
				pl.setDatabaseDefaults();
				mode->subEntityTraits().setColor(256);
				pl.worldDraw(mode);
				mode->subEntityTraits().setColor(3);
				AcDbLine(rect_pillar_vertex_.at(0), rect_pillar_vertex_.at(2)).worldDraw(mode);
				AcDbLine(rect_pillar_vertex_.at(1), rect_pillar_vertex_.at(3)).worldDraw(mode);
			}
		}
		else
		{
			return Adesk::kFalse;
		}
	}
	else
	{
		return Adesk::kFalse;
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
	if (!checkValue(this) || (pillar_type_ == 1 && rect_pillar_vertex_.length() != 4)) return Acad::eInvalidInput;
	Acad::ErrorStatus error_status = Acad::eInvalidInput;
	if (pillar_type_ == 0)
	{
		AcDbCircle contour_circle(center_point_, AcGeVector3d::kZAxis, pillar_d_ * 0.5);
		error_status = contour_circle.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
	}
	else if (pillar_type_ == 1)
	{
		AcDbPolyline contour_polyline;
		error_status = buildPillarContour(rect_pillar_vertex_, contour_polyline);
		if (error_status != Acad::eOk) return error_status;
		error_status = contour_polyline.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
	}
	else
	{
		return Acad::eInvalidInput;
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
	if (!checkValue(this) || (pillar_type_ == 1 && rect_pillar_vertex_.length() != 4)) return Acad::eInvalidInput;
	Acad::ErrorStatus error_status = Acad::eInvalidInput;
	if (pillar_type_ == 0)
	{
		AcDbCircle contour_circle(center_point_, AcGeVector3d::kZAxis, pillar_d_ * 0.5);
		error_status = contour_circle.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
	}
	else if (pillar_type_ == 1)
	{
		AcDbPolyline contour_polyline;
		error_status = buildPillarContour(rect_pillar_vertex_, contour_polyline);
		if (error_status != Acad::eOk) return error_status;
		error_status = contour_polyline.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat);
	}
	else
	{
		return Acad::eInvalidInput;
	}
	return error_status;
}

//- Grip points protocol
Acad::ErrorStatus CPolaCustomPillar::subGetGripPoints(
	AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds
) const {
	assertReadEnabled();
	if (!checkValue(this) || (pillar_type_ == 1 && rect_pillar_vertex_.length() != 4)) return Acad::eInvalidInput;
	//----- This method is never called unless you return eNotImplemented
	//----- from the new getGripPoints() method below (which is the default implementation)
	if (pillar_type_ == 0)
	{
		gripPoints.append(center_point_);
	}
	else if (pillar_type_ == 1)
	{
		gripPoints.append(center_point_);
		for (int i = 0; i < rect_pillar_vertex_.length(); i++)
		{
			gripPoints.append(rect_pillar_vertex_.at(i));
		}
	}
	else
	{
		return Acad::eInvalidInput;
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

Acad::ErrorStatus CPolaCustomPillar::setCenterPoint(const AcGePoint3d & center)
{
	assertWriteEnabled();
	if (!finitePoint(center)) return Acad::eInvalidInput;
	const AcGePoint3d previous = center_point_;
	center_point_ = center;
	const Acad::ErrorStatus status = CalculateVertex();
	if (status != Acad::eOk) center_point_ = previous;
	return status;
}

Acad::ErrorStatus CPolaCustomPillar::setDirectionVector(const AcGeVector3d & dir_vec)
{
	assertWriteEnabled();
	if (!finiteDirection(dir_vec)) return Acad::eInvalidInput;
	const AcGeVector3d previous = direction_vector_;
	direction_vector_ = dir_vec;
	const Acad::ErrorStatus status = CalculateVertex();
	if (status != Acad::eOk) direction_vector_ = previous;
	return status;
}

Acad::ErrorStatus CPolaCustomPillar::setDiameter(const double& d, const double& h)
{
	assertWriteEnabled();
	if (!std::isfinite(d) || !std::isfinite(h) || d <= 0.0 || h < 0.0
		|| (pillar_type_ == 1 && h <= 0.0)) return Acad::eInvalidInput;
	const double previous_d = pillar_d_, previous_h = pillar_h_;
	pillar_d_ = d;
	pillar_h_ = h;
	const Acad::ErrorStatus status = CalculateVertex();
	if (status != Acad::eOk) { pillar_d_ = previous_d; pillar_h_ = previous_h; }
	return status;
}

Acad::ErrorStatus CPolaCustomPillar::setViewable(const bool view)
{
	assertWriteEnabled();
	viewable_ = view;
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomPillar::setPillarProperty(const Adesk::Int32 & prop)
{
	assertWriteEnabled();
	if (prop == 0)
		pillar_property_ = prop;
	else if (prop == 1)
		pillar_property_ = prop;
	else
		return Acad::eInvalidInput;
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomPillar::setSn(const TCHAR * sn)
{
	assertWriteEnabled();
	if (!sn || _tcslen(sn) > kMaxSerialLength) return Acad::eInvalidInput;
	pillar_serial_number_ = sn;
	return Acad::eOk;
}

Acad::ErrorStatus CPolaCustomPillar::setPillarType(const Adesk::Int32 & type)
{
	assertWriteEnabled();
	if ((type != 0 && type != 1) || (type == 1 && pillar_h_ <= 0.0)) return Acad::eInvalidInput;
	const Adesk::Int32 previous = pillar_type_;
	if (type == 0)
		pillar_type_ = type;
	else if (type == 1)
		pillar_type_ = type;
	else
		return Acad::eInvalidInput;
	const Acad::ErrorStatus status = CalculateVertex();
	if (status != Acad::eOk) pillar_type_ = previous;
	return status;
}

Acad::ErrorStatus CPolaCustomPillar::setConcreteGrade(const Adesk::Int32 & grade)
{
	assertWriteEnabled();
	if (grade < 0) return Acad::eInvalidInput;
	concrete_grade_ = grade;
	return Acad::eOk;
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

const TCHAR* CPolaCustomPillar::getPillarSn() const
{
	assertReadEnabled();
	return pillar_serial_number_.c_str();
}

Adesk::Int32 CPolaCustomPillar::getPillarType() const
{
	assertReadEnabled();
	return pillar_type_;
}

Adesk::Int32 CPolaCustomPillar::getConcreteGrade() const
{
	assertReadEnabled();
	return concrete_grade_;
}

AcGePoint3dArray CPolaCustomPillar::getRectVertex() const
{
	assertReadEnabled();
	return rect_pillar_vertex_;
}

Acad::ErrorStatus CPolaCustomPillar::CalculateVertex()
{
	assertWriteEnabled();
	if (!checkValue(this)) return Acad::eInvalidInput;
	AcGePoint3dArray next_vertices;
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
				if (!finitePoint(temp_vertex[i])) return Acad::eInvalidInput;
				next_vertices.append(temp_vertex[i]);
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
				if (!finitePoint(temp_vertex[i])) return Acad::eInvalidInput;
				next_vertices.append(temp_vertex[i]);
			}
		}
	}
	rect_pillar_vertex_ = next_vertices;
	return Acad::eOk;
}

void CPolaCustomPillar::UpdateEntity()
{

}

Acad::ErrorStatus CPolaCustomPillar::subTransformBy(const AcGeMatrix3d & transform_matrix)
{
	assertWriteEnabled();
	if (!checkValue(this)) return Acad::eInvalidInput;
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			if (!std::isfinite(transform_matrix.entry[row][col])) return Acad::eInvalidInput;
	// The stored primitives are planar. Reject shear/tilt and circle distortion.
	if (!transform_matrix.isScaledOrtho() || transform_matrix.isSingular()
		|| (pillar_type_ == 0 && !transform_matrix.isUniScaledOrtho())) return Acad::eInvalidInput;
	const AcGeVector3d normal = transform_matrix * AcGeVector3d::kZAxis;
	if (!normal.isParallelTo(AcGeVector3d::kZAxis)) return Acad::eInvalidInput;
	const AcGePoint3d next_center = transform_matrix * center_point_;
	const AcGeVector3d next_direction = transform_matrix * direction_vector_;
	const double scale_d = (transform_matrix * direction_vector_.normal()).length();
	const AcGeVector3d width_direction(-direction_vector_.y, direction_vector_.x, 0.0);
	const double scale_h = (transform_matrix * width_direction.normal()).length();
	const double next_d = pillar_d_ * scale_d;
	const double next_h = pillar_h_ * scale_h;
	if (!finitePoint(next_center) || !finiteDirection(next_direction)
		|| !std::isfinite(next_d) || next_d <= 0.0 || !std::isfinite(next_h)
		|| next_h < 0.0 || (pillar_type_ == 1 && next_h <= 0.0)) return Acad::eInvalidInput;
	AcGePoint3dArray next_vertices = rect_pillar_vertex_;
	for (int i = 0; i < next_vertices.length(); ++i) {
		next_vertices[i].transformBy(transform_matrix);
		if (!finitePoint(next_vertices[i])) return Acad::eInvalidInput;
	}
	center_point_ = next_center;
	direction_vector_ = next_direction;
	pillar_d_ = next_d;
	pillar_h_ = next_h;
	rect_pillar_vertex_ = next_vertices;
	return Acad::eOk;
}

bool CPolaCustomPillar::checkValue(const CPolaCustomPillar * pillar)
{
	if (!pillar) return false;
	double d, h;
	pillar->getDiameter(d, h);
	bool size_result = std::isfinite(d) && std::isfinite(h) && d > 0 && h >= 0
		&& (pillar->getPillarType() != 1 || h > 0);
	bool prop_result = (pillar->getPillarProperty() == 0 || pillar->getPillarProperty() == 1) ? true : false;
	bool type_result = (pillar->getPillarType() == 0 || pillar->getPillarType() == 1) ? true : false;

	return size_result && prop_result && type_result && finitePoint(pillar->getCenterPoint())
		&& finiteDirection(pillar->getDirectionVector()) && pillar->getConcreteGrade() >= 0
		&& pillar->pillar_serial_number_.length() <= kMaxSerialLength;
}

AcDbObjectIdArray CPolaCustomPillar::BatchInsert(const CPolaCustomPillar & pillar_template, const AcGePoint3dArray & insert_point_array)
{
	AcDbObjectIdArray pillar_id_array;
	const Acad::ErrorStatus status = BatchInsert(pillar_template, insert_point_array, pillar_id_array);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar insertion failed: %d"), static_cast<int>(status));
	return pillar_id_array;
}

Acad::ErrorStatus CPolaCustomPillar::BatchInsert(const CPolaCustomPillar& pillar_template,
	const AcGePoint3dArray& insert_points, AcDbObjectIdArray& pillar_ids)
{
	pillar_ids.removeAll();
	if (!checkValue(&pillar_template) || insert_points.isEmpty()) return Acad::eInvalidInput;
	for (int i = 0; i < insert_points.length(); ++i)
		if (!finitePoint(insert_points[i])) return Acad::eInvalidInput;
	AcDbObjectIdArray inserted;
	for (int i = 0; i < insert_points.length(); ++i) {
		AcDbObjectId id;
		const Acad::ErrorStatus status = SingleInsert(pillar_template, insert_points[i], id);
		if (status != Acad::eOk) {
			// Roll back earlier appends; never hide the first append failure.
			for (int j = 0; j < inserted.length(); ++j) {
				AcDbEntity* entity = nullptr;
				const Acad::ErrorStatus open_status = acdbOpenObject(entity, inserted[j], AcDb::kForWrite);
				if (open_status == Acad::eOk) {
					Pola::OpenedObject<AcDbEntity> opened(entity);
					const Acad::ErrorStatus erase_status = entity->erase();
					const Acad::ErrorStatus cleanup_status = Pola::Close(opened, erase_status);
					if (cleanup_status != Acad::eOk) acutPrintf(_T("\nPillar insertion rollback failed: %d"), static_cast<int>(cleanup_status));
				}
				else acutPrintf(_T("\nPillar insertion rollback open failed: %d"), static_cast<int>(open_status));
			}
			return status;
		}
		inserted.append(id);
	}
	pillar_ids = inserted;
	return Acad::eOk;
}

AcDbObjectId CPolaCustomPillar::SingleInsert(const CPolaCustomPillar & pillar_template, const AcGePoint3d & insert_point)
{
	AcDbObjectId id;
	const Acad::ErrorStatus status = SingleInsert(pillar_template, insert_point, id);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar insertion failed: %d"), static_cast<int>(status));
	return id;
}

Acad::ErrorStatus CPolaCustomPillar::SingleInsert(const CPolaCustomPillar& pillar_template,
	const AcGePoint3d& insert_point, AcDbObjectId& pillar_id)
{
	pillar_id = AcDbObjectId::kNull;
	if (!checkValue(&pillar_template) || !finitePoint(insert_point)) return Acad::eInvalidInput;
	std::unique_ptr<CPolaCustomPillar> pillar(new CPolaCustomPillar(pillar_template));
	Acad::ErrorStatus status = pillar->setCenterPoint(insert_point);
	if (status != Acad::eOk) return status;
	status = AddToModelSpace::AddEntityToModelSpace(pillar.get(), pillar_id, false);
	// Inspect ownership while the handle is still open, then close exactly once.
	if (!pillar->objectId().isNull()) {
		if (status != Acad::eOk) {
			const Acad::ErrorStatus rollback_status = pillar->erase();
			if (rollback_status != Acad::eOk) acutPrintf(_T("\nPillar insertion rollback failed: %d"), static_cast<int>(rollback_status));
		}
		const Acad::ErrorStatus close_status = pillar.release()->close();
		if (status == Acad::eOk) status = close_status;
	}
	if (status != Acad::eOk) pillar_id = AcDbObjectId::kNull;
	return status;
}

AcDbObjectId CPolaCustomPillar::AddPillarLeader()
{
	AcDbObjectId id;
	const Acad::ErrorStatus status = AddPillarLeader(id);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar leader failed: %d"), static_cast<int>(status));
	return id;
}

Acad::ErrorStatus CPolaCustomPillar::AddPillarLeader(AcDbObjectId& leader_id)
{
	assertReadEnabled();
	leader_id = AcDbObjectId::kNull;
	if (!checkValue(this)) return Acad::eInvalidInput;
	AcGePoint3d point_on_leader(center_point_.x + 2000, center_point_.y - 1000, center_point_.z);
	AcGePoint3d text_point(point_on_leader.x + 2000, point_on_leader.y, point_on_leader.z);
	std::wstring info;
	std::wstringstream info_stream;
	if (pillar_type_ == 1)
	{
		info_stream << pillar_serial_number_ << _T("\n") << pillar_d_ << _T("*") << pillar_h_;
		info = info_stream.str();
	}
	else if (pillar_type_ == 0)
	{
		info_stream << pillar_serial_number_ << _T("\n%%c") << pillar_d_;
		info = info_stream.str();
	}
	else
	{
		return Acad::eInvalidInput;
	}
	return DrawEntity::AddMLeader(center_point_, point_on_leader, text_point, info.c_str(), leader_id);
}

Acad::ErrorStatus CPolaCustomPillar::subGetGeomExtents(AcDbExtents & extents) const
{
	assertReadEnabled();
	if (!checkValue(this) || (pillar_type_ == 1 && rect_pillar_vertex_.length() != 4)) return Acad::eInvalidInput;
	Acad::ErrorStatus error_status = Acad::eInvalidInput;
	if (pillar_type_ == 0)
	{
		AcDbCircle circle(center_point_, AcGeVector3d::kZAxis, pillar_d_ * 0.5);
		error_status = circle.getGeomExtents(extents);
	}
	else if (pillar_type_ == 1)
	{
		AcDbPolyline pl;
		error_status = buildPillarContour(rect_pillar_vertex_, pl);
		if (error_status != Acad::eOk) return error_status;
		pl.setDatabaseDefaults();
		error_status = pl.getGeomExtents(extents);
	}
	else
	{
		return Acad::eInvalidInput;
	}
	return error_status;
}
