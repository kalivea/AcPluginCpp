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
//----- Shielding.cpp : Implementation of CShielding
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "Shielding.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CShielding, CAcUiTabChildDialog)

BEGIN_MESSAGE_MAP(CShielding, CAcUiTabChildDialog)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CShielding::CShielding (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiTabChildDialog (pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CShielding::DoDataExchange (CDataExchange *pDX) {
	CAcUiTabChildDialog::DoDataExchange (pDX) ;
}

//-----------------------------------------------------------------------------
//----- Called when this tab is activated. The default implementation does nothing
void CShielding::OnTabActivation (BOOL bActivate) {
	// TODO: Add your code here
}

//-----------------------------------------------------------------------------
//----- Called when a tab is de-activated and another is about to be activated. 
//----- The default implementation returns TRUE, allowing the change. 
//----- Return FALSE to prevent the tab switch.
BOOL CShielding::OnTabChanging () {
	// TODO: Add your code here

	return (TRUE) ;
}
