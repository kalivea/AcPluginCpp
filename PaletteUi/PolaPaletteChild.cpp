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
//----- PolaPaletteChild.cpp : Implementation of CPolaPaletteChild
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaPaletteChild.h"
#include "DrawEntity.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CPolaPaletteChild, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaPaletteChild, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_PRINT_HELLO, &CPolaPaletteChild::OnBnClickedPrintHello)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaPaletteChild::CPolaPaletteChild (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog (CPolaPaletteChild::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CPolaPaletteChild::DoDataExchange (CDataExchange *pDX) {
	CAdUiBaseDialog::DoDataExchange (pDX) ;
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaPaletteChild::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CPolaPaletteChild::OnBnClickedPrintHello()
{
	acDocManager->lockDocument(acDocManager->curDocument());
	// TODO: 在此添加控件通知处理程序代码
	DrawEntity::DrawCircle(AcGePoint3d(0, 0, 0), 100);
	acDocManager->unlockDocument(acDocManager->curDocument());
}
