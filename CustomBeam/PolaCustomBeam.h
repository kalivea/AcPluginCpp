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
//----- PolaCustomBeam.h : Declaration of the CPolaCustomBeam
//-----------------------------------------------------------------------------
#pragma once

#ifdef CUSTOMBEAM_MODULE
#define BEAM_DLLIMPEXP __declspec(dllexport)
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
#define BEAM_DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include <sstream> 
//-----------------------------------------------------------------------------
class BEAM_DLLIMPEXP CPolaCustomBeam : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(CPolaCustomBeam);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	CPolaCustomBeam();
	virtual ~CPolaCustomBeam();

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
	//-----------------------------------------------------------------------------
	// custom define pillar part             ------Pola

private:
	// save to dwg files.
	AcGePoint3dArray beam_vertexes_;				// store  vertexes of beam.
	AcGePoint3dArray top_offset_vertex_;
	AcGePoint3dArray bottom_offset_vertex_;

	Adesk::Int32 vertexes_num_ = 0;					// store number of vertexes.

	double beam_b_;								    // store width of beam.	
	double beam_h_;								    // store height of beam.

	std::vector<Adesk::Int32> beam_viewable_;		// store viewable of beam.

	Adesk::Int32 beam_property_;					// store property of beam. The sn of beam.
	AcGeVector3dArray beam_segment_direction;		// store beam segment direction.
public:
	//           get functions
	AcGePoint3dArray getBeamVertexes() const;
	double getBeamWidth() const;
	double getBeamHeight() const;
	std::vector<Adesk::Int32> getBeamViewable() const;
	Adesk::Int32 getBeamProperty() const;
	Adesk::Int32 getVertexesNum() const;
	AcGeVector3dArray getBeamSegmentDirection() const;
	//
	double getBeamLength() const;
private:
	AcGePoint3d getHorizontalMidPoint() const;
	//           set functions
public:
	void setBeamVertexes(const AcGePoint3dArray& beam_vertexes);
	void setBeamWidth(const double& beam_b);
	void setBeamHeight(const double& beam_h);
	void setBeamViewable(const std::vector<Adesk::Int32>& beam_viewable);
	void setBeamProperty(const Adesk::Int32& beam_property);
	void setBeamSegmentDireciton(const AcGeVector3dArray& segment_direction);
	//			add a single centerline vertex, calculate the offset line veretx.
private:
	void addVertexAt(const int index, const AcGePoint3d& vertex);
	void addViewableAt(const int index, const Adesk::Int32 viewable);
	void resetViewableAt(const int index, const Adesk::Int32 viewable);
	void UpdateOffsetLine(const double& distance);
	void GenerateBeamSegmentDirection();
	static AcDbObjectId DrawBeamWithOffset(CPolaCustomBeam* beam, const double offset_distance);
	// 
public:
	static AcDbObjectId PickCenterPointDrawBeam(CPolaCustomBeam* beam);
	static AcDbObjectId PickTopPointDrawBeam(CPolaCustomBeam* beam);
	static AcDbObjectId PickBottomPointDrawBeam(CPolaCustomBeam* beam);
	static AcDbObjectId SelectPillarDrawBeam(CPolaCustomBeam* beam);

	static bool ModifyViewable(CPolaCustomBeam* beam, int index, Adesk::Int32 viewable);
	static bool ModifyViewable(AcDbObjectId beam_id, int index, Adesk::Int32 viewable);
	// 
public:
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xfrom);
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;
	//
public:
	AcDbObjectIdArray GetIntersectingPillar() const;
	Adesk::Int32 GetSegmentIndexByYProjection(const AcGePoint3d& point, const AcGeTol& tol = AcGeContext::gTol) const;
	void addJoint(const double slab_thickness, const double offset_length = 100);
	AcDbObjectId addBeamSnInfo();
	Acad::ErrorStatus InsertVertex(const AcGePoint3d& insert_point);			// TODO: A tolerance value is required to determine 
																				// whether the beam is within a certain range to prevent user error.
	static AcDbObjectId genbeam();
};

#ifdef CUSTOMBEAM_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CPolaCustomBeam)
#endif
