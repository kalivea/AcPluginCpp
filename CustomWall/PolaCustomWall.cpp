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
//----- PolaCustomWall.cpp : Implementation of CPolaCustomWall
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PolaCustomWall.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CPolaCustomWall::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CPolaCustomWall, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, POLACUSTOMWALL,
POLACUSTOMWALLAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
CPolaCustomWall::CPolaCustomWall () : AcDbEntity () {
}

CPolaCustomWall::~CPolaCustomWall () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CPolaCustomWall::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CPolaCustomWall::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CPolaCustomWall::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CPolaCustomWall::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CPolaCustomWall::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CPolaCustomWall::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	return (AcDbEntity::subWorldDraw (mode)) ;
}


Adesk::UInt32 CPolaCustomWall::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}


