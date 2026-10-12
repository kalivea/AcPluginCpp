// (C) Copyright 2002-2012 by Autodesk, Inc.
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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaReactor.h"
#include "CallbackManager.h"
#include <tchar.h>
#include "../Common/DbObjectOwner.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("Pola")

void syncRadius(const AcDbObject* source, AcDbObjectId target)
{
	const AcDbCircle* circ = AcDbCircle::cast(source);
	if (!circ || target.isNull() || source->objectId() == target) return;
	AcDbCircle* circ_t = nullptr;
	double radius = circ->radius();
	const Acad::ErrorStatus open_status = acdbOpenObject(circ_t, target, AcDb::kForWrite);
	if (open_status == Acad::eOk)
	{
		Pola::OpenedObject<AcDbCircle> target_circle(circ_t);
		const Acad::ErrorStatus status = Pola::Close(target_circle, circ_t->setRadius(radius));
		if (status != Acad::eOk) acutPrintf(_T("\nRadius synchronization failed: %d"), static_cast<int>(status));
	}
	else acutPrintf(_T("\nRadius synchronization open failed: %d"), static_cast<int>(open_status));
}

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint

class CEntityReactorApp : public AcRxDbxApp {

public:
	CEntityReactorApp() : AcRxDbxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxDbxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here
		CPolaReactor::rxInit();
		CallbackManager::getInstance();
		CallbackManager::getInstance().registerCallback(1, syncRadius);
		acrxBuildClassHierarchy();
		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here
		deleteAcRxClass(CPolaReactor::desc());
		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEntityReactorApp)
