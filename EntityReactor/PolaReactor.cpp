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
//----- PolaReactor.cpp : Implementation of CPolaReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PolaReactor.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CPolaReactor::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	CPolaReactor, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, POLAREACTOR,
	POLAENTITYREACTORAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
CPolaReactor::CPolaReactor() : AcDbObject() {
}

CPolaReactor::CPolaReactor(AcDbObjectId & boundObjectId, int callbackId)
	: bound_objectId(boundObjectId), callback_id(callbackId)
{
}

CPolaReactor::~CPolaReactor()
{
}
//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CPolaReactor::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(CPolaReactor::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....
	es = pFiler->writeItem(callback_id);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem((AcDbSoftPointerId&)bound_objectId);
	if (es != Acad::eOk)
		return (es);
	return (pFiler->filerStatus());
}

Acad::ErrorStatus CPolaReactor::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > CPolaReactor::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CPolaReactor::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	es = pFiler->readItem(&callback_id);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->readItem((AcDbSoftPointerId*)&bound_objectId);
	if (es != Acad::eOk)
		return (es);
	return (pFiler->filerStatus());
}

void CPolaReactor::modified(const AcDbObject * pDbObj)
{
	trigger(pDbObj, bound_objectId);
	AcDbObject::modified(pDbObj);
}

void CPolaReactor::trigger(const AcDbObject * source_id, AcDbObjectId target_id) const
{
	CallbackManager::getInstance().executeCallback(callback_id, source_id, target_id);
}

inline void CPolaReactor::BoundObjectId(AcDbObjectId & id)
{
	bound_objectId = id;
}

AcDbObjectId CPolaReactor::boundObjectId() const
{
	return bound_objectId;
}

