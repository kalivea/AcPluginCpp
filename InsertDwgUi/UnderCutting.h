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
//----- UnderCutting.h : Declaration of the CUnderCutting
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include "resource.h"
//-----------------------------------------------------------------------------
class CUnderCutting : public CAcUiTabChildDialog {		
	DECLARE_DYNAMIC(CUnderCutting)

public:
	CUnderCutting(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);

	enum { IDD = IDD_UNDERCUT_PAGE };

public:
	//----- Called when this tab is activated.
	virtual void OnTabActivation(BOOL bActivate);
	//----- Called when a tab is de-activated and another is about to be activated. 
	virtual BOOL OnTabChanging();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
