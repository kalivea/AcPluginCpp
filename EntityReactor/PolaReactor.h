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
//----- PolaReactor.h : Declaration of the CPolaReactor
//-----------------------------------------------------------------------------
#pragma once

#ifdef ENTITYREACTOR_MODULE
#define DLLIMPEXP __declspec(dllexport)
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
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include "CallbackManager.h"
//-----------------------------------------------------------------------------
class DLLIMPEXP CPolaReactor : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(CPolaReactor);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	CPolaReactor();
	CPolaReactor(AcDbObjectId& boundObjectId, int callbackId);
	virtual ~CPolaReactor();

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	virtual void modified(const AcDbObject* pDbObj) override;
	void trigger(const AcDbObject* source_object, AcDbObjectId target_id) const;
	void BoundObjectId(AcDbObjectId& id);
	AcDbObjectId boundObjectId() const;
private:
	AcDbObjectId bound_objectId;
	int callback_id;
};

#ifdef ENTITYREACTOR_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CPolaReactor)
#endif
