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
#include "../Common/DrawEntity.h"
namespace
{
struct DocumentLockCleanup
{
	AcApDocument* document;
	~DocumentLockCleanup() { if (document != nullptr) acDocManager->unlockDocument(document); }
	Acad::ErrorStatus close()
	{
		AcApDocument* locked = document;
		document = nullptr;
		return acDocManager->unlockDocument(locked);
	}
};
}
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CPolaPaletteChild, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaPaletteChild, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_PRINT_HELLO, &CPolaPaletteChild::OnBnClickedPrintHello)
	ON_WM_PAINT()
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
	AcApDocument* document = acDocManager->curDocument();
	if (document == nullptr) return;
	Acad::ErrorStatus status = acDocManager->lockDocument(document);
	if (status != Acad::eOk)
	{
		acutPrintf(_T("\nFailed to lock document: %d"), static_cast<int>(status));
		return;
	}
	DocumentLockCleanup lock{ document };
	AcDbObjectId id;
	status = DrawEntity::DrawCircle(AcGePoint3d(0, 0, 0), 100, id);
	const Acad::ErrorStatus cleanup = lock.close();
	if (status == Acad::eOk) status = cleanup;
	if (status != Acad::eOk) acutPrintf(_T("\nPalette drawing failed: %d"), static_cast<int>(status));
}

void CPolaPaletteChild::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CAdUiBaseDialog::OnPaint()
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(59, 68, 83));		// cad dark mode background color
	CAdUiBaseDialog::OnPaint();
}
