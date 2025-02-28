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
//----- SyncCircleReactor.cpp : Implementation of SyncCircleReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "SyncCircleReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(SyncCircleReactor, AcDbObjectReactor, 1)

SyncCircleReactor::SyncCircleReactor()
{
}

//-----------------------------------------------------------------------------
SyncCircleReactor::SyncCircleReactor(AcDbObjectId otherId)
    : m_otherCircleId(otherId)
{
}

SyncCircleReactor::~SyncCircleReactor()
{
    acutPrintf(_T("\nReactor delete"));
}

void SyncCircleReactor::modified(const AcDbObject* dbObj)
{
    AcDbCircle* pModifiedCircle = AcDbCircle::cast(dbObj);
    if (!pModifiedCircle || m_otherCircleId.isNull()) return;

    acutPrintf(_T("\ncircle modified"));
    AcDbExtents extents1;
    pModifiedCircle->getGeomExtents(extents1);
 
    double newRadius = pModifiedCircle->radius();

    AcDbCircle* pOtherCircle = nullptr;
    if (acdbOpenObject(pOtherCircle, m_otherCircleId, AcDb::kForWrite) == Acad::eOk)
    {
        pOtherCircle->removeReactor(this);
        AcDbExtents extents2;
        pOtherCircle->getGeomExtents(extents2); 

        if (BasicTools::IsIntersectRectangle(extents1, extents2))
        {
            pOtherCircle->setColorIndex(1);
        }
        else
        {
            pOtherCircle->setColorIndex(255);
        }
        pOtherCircle->setRadius(newRadius);
        acutPrintf(_T("\nsync radius: %f"), newRadius);

        pOtherCircle->addReactor(this);
        pOtherCircle->close();
    }
}

