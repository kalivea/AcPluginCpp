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
//----- PolaDockCtrlBar.cpp : Implementation of CPolaDockCtrlBar
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaDockCtrlBar.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CPolaDockCtrlBar, CAcUiDockControlBar)

BEGIN_MESSAGE_MAP(CPolaDockCtrlBar, CAcUiDockControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//----- CPolaDockCtrlBar *pInstance = new CPolaDockCtrlBar;
//----- pInstance->Create (acedGetAcadFrame (), "My title bar") ;
//----- pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
//----- pInstance->RestoreControlBar () ;

//-----------------------------------------------------------------------------
static CLSID clsCPolaDockCtrlBar = {0xeab78c04, 0x2194, 0x47ad, {0xa4, 0xf2, 0xad, 0xca, 0x3e, 0x3b, 0xb6, 0x3c}} ;


//-----------------------------------------------------------------------------
CPolaDockCtrlBar::CPolaDockCtrlBar () : CAcUiDockControlBar() {
}

//-----------------------------------------------------------------------------
CPolaDockCtrlBar::~CPolaDockCtrlBar () {
}

//-----------------------------------------------------------------------------
#ifdef _DEBUG
//- Please uncomment the 2 following lines to avoid linker error when compiling
//- in release mode. But make sure to uncomment these lines only once per project
//- if you derive multiple times from CAdUiDockControlBar/CAcUiDockControlBar
//- classes.

//void CAdUiDockControlBar::AssertValid () const {
//}
#endif

//-----------------------------------------------------------------------------
BOOL CPolaDockCtrlBar::Create (CWnd *pParent, LPCSTR lpszTitle) {
	CString strWndClass ;
	strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;
	CRect rect (0, 0, 250, 200) ;
	if (!CAcUiDockControlBar::Create (
			strWndClass, (LPCTSTR)lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
			rect, pParent, 0
		)
	)
		return (FALSE) ;

	SetToolID (&clsCPolaDockCtrlBar) ;

	// TODO: Add your code here
	
	return (TRUE) ;
}

//-----------------------------------------------------------------------------
//----- This member function is called when an application requests the window be 
//----- created by calling the Create or CreateEx member function
int CPolaDockCtrlBar::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if ( CAcUiDockControlBar::OnCreate (lpCreateStruct) == -1 )
		return (-1) ;

	return (0) ;
}

//-----------------------------------------------------------------------------
void CPolaDockCtrlBar::SizeChanged (CRect *lpRect, BOOL bFloating, int flags) {
}

