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
//----- CircleReactor.cpp : Implementation of CCircleReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CircleReactor.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CCircleReactor::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	CCircleReactor, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, CIRCLEREACTOR,
	POLAENTITYREACTORAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
CCircleReactor::CCircleReactor() : AcDbObject() {
}

CCircleReactor::~CCircleReactor() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CCircleReactor::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(CCircleReactor::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....
	es = pFiler->writeItem((AcDbSoftPointerId&)entity_id_);
	if (es != Acad::eOk)
		return (es);
	return (pFiler->filerStatus());
}

Acad::ErrorStatus CCircleReactor::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > CCircleReactor::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CCircleReactor::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	es = pFiler->readItem((AcDbSoftPointerId*)&entity_id_);
	if (es != Acad::eOk)
		return (es);
	return (pFiler->filerStatus());
}

//- Persistent reactor callbacks
void CCircleReactor::erased(const AcDbObject * pDbObj, Adesk::Boolean bErasing) {
	assertReadEnabled();
	AcDbObject::erased(pDbObj, bErasing);
}

void CCircleReactor::modified(const AcDbObject * pDbObj) {
	assertReadEnabled();
	AcDbCircle* circ = AcDbCircle::cast(pDbObj);
	AcDbCircle* circ_t = nullptr;
	double radius = circ->radius();
	if (acdbOpenObject(circ_t, entity_id_, OpenMode::kForWrite) == Acad::eOk) 
	{
		circ_t->setRadius(radius);
		circ_t->close();
	}
	AcDbObject::modified(pDbObj);
}

void CCircleReactor::setLink(AcDbObjectId entity_id)
{
	assertReadEnabled();
	entity_id_ = entity_id;
}



