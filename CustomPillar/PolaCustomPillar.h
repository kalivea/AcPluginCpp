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
//----- PolaCustomPillar.h : Declaration of the CPolaCustomPillar
//-----------------------------------------------------------------------------
#pragma once

#ifdef CUSTOMPILLAR_MODULE
#define PILLAR_DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define PILLAR_DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"

//-----------------------------------------------------------------------------
class PILLAR_DLLIMPEXP CPolaCustomPillar : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(CPolaCustomPillar);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	CPolaCustomPillar();
	CPolaCustomPillar(const CPolaCustomPillar& pillar_template);
	virtual ~CPolaCustomPillar();

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits* traits);

	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d& pickPoint,
		const AcGePoint3d& lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray& snapPoints,
		AcDbIntArray& geomIds) const;
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d& pickPoint,
		const AcGePoint3d& lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray& snapPoints,
		AcDbIntArray& geomIds,
		const AcGeMatrix3d& insertionMat) const;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds) const;
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset);
	virtual Acad::ErrorStatus subGetGripPoints(
		AcDbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize,
		const AcGeVector3d& curViewDir, const int bitflags) const;
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbVoidPtrArray& gripAppData, const AcGeVector3d& offset, const int bitflags);
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;		// get pillar geometry extents.
	//-----------------------------------------------------------------------------
	// custom define pillar part             ------Pola
private:
	//					save to XData
	AcGePoint3d center_point_;					// center point: Used to define the insertion point of the entity in the drawing file.
	AcGeVector3d direction_vector_;				// direction vercor: The angle between the center point and the x-axis, used to define the positive direction of the entity.
	double pillar_d_, pillar_h_;				// Diameter of the entity: If it is a rectangular pillar, d and h represent the length and width.
	//						   If it is a circular pillar, d = h, represent the diameter of the circle.
	bool viewable_;								// viewalbe: Used to control the entity line type. true mean continuous; false mean dashed.
	Adesk::Int32 pillar_property_;				// pillar property: (int) 0 mean concrete; (int) 1 mean concrete-filled steel tube pillar.
	Adesk::Int32 pillar_serial_number_;			// pillar serial number.
	Adesk::Int32 pillar_type_;					// pillar type: (int) 0 mean round pillar; (int) 1 mean rectangular pillar.

	//	               auxiliary data
	AcGePoint3dArray rect_pillar_vertex_;		// vertex: store rectangular pillar vertex.
	Adesk::Int32 concrete_grade_;

public:
	//					set function
	void setCenterPoint(const AcGePoint3d& center);
	void setDirectionVector(const AcGeVector3d& dir_vec = AcGeVector3d::kXAxis);
	void setDiameter(const double& d, const double& h);
	void setViewable(const bool view);
	void setPillarProperty(const Adesk::Int32& prop);
	void setSn(const Adesk::Int32& sn);
	void setPillarType(const Adesk::Int32& type);
	void setConcreteGrade(const Adesk::Int32& grade);

	//					get function
	AcGePoint3d getCenterPoint() const;
	AcGeVector3d getDirectionVector() const;
	void getDiameter(double& d, double& h) const;
	bool getViewable() const;
	Adesk::Int32 getPillarProperty() const;
	Adesk::Int32 getPillarSn() const;
	Adesk::Int32 getPillarType() const;
	Adesk::Int32 getConcreteGrade() const;

	//				   auxiliary data generation function
	void CalculateVertex();
	void UpdateEntity();			// TODO

	//                  move transform
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& transform_matrix);
	//virtual Acad::ErrorStatus subGetTransformedCopy(AcGeMatrix3d transform_atrix, AcDbEntity* entity);

	//					init pillar and value check
	static bool checkValue(const CPolaCustomPillar* pillar);
	static AcDbObjectIdArray BatchInsert(const CPolaCustomPillar& pillar_template, const AcGePoint3dArray& insert_point_array);
	static AcDbObjectId SingleInsert(const CPolaCustomPillar& pillar_template, const AcGePoint3d& insert_point);
	AcDbObjectId AddPillarLeader();

	//
};

#ifdef CUSTOMPILLAR_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CPolaCustomPillar)
#endif
