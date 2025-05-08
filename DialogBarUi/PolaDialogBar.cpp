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
//----- PolaDialogBar.cpp : Implementation of CPolaDialogBar
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaDialogBar.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CPolaDialogBar, CAdUiDialogBar)

BEGIN_MESSAGE_MAP(CPolaDialogBar, CAdUiDialogBar)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(64000, OnButtonClicked)
	ON_COMMAND_RANGE(ID_MENU_ITEM_1, ID_MENU_ITEM_3, OnMenuSelect)

END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaDialogBar::CPolaDialogBar (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiDialogBar (/*CPolaDialogBar::IDD, pParent, hInstance*/) {
}

BOOL CPolaDialogBar::Create(CFrameWnd* pParentWnd, CString csName)
{
	CAcModuleResourceOverride Vtemp;

	if (!CAdUiDialogBar::Create(pParentWnd, IDD_POLADIALOGBAR, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM, AFX_IDW_CONTROLBAR_LAST))
	{
		return FALSE;
	}
	SetBorders();
	SetWindowText(csName);
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableWindow(true);
	GetDockingFrame()->ShowControlBar(this, TRUE, FALSE);
	GetDockingFrame()->RecalcLayout();
	return TRUE;
}

//-----------------------------------------------------------------------------
void CPolaDialogBar::DoDataExchange (CDataExchange *pDX) {
	CAdUiDialogBar::DoDataExchange (pDX) ;
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaDialogBar::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CPolaDialogBar::OnMenuSelect(UINT nID)
{
	switch (nID)
	{
	case ID_MENU_ITEM_1:
		AfxMessageBox(_T("你选择了菜单项 1"));
		break;
	case ID_MENU_ITEM_2:
		AfxMessageBox(_T("你选择了菜单项 2"));
		break;
	case ID_MENU_ITEM_3:
		AfxMessageBox(_T("你选择了菜单项 3"));
		break;
	}
}

void CPolaDialogBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CAdUiDialogBar::OnPaint()
	
}

void CPolaDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CAdUiDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

int CPolaDialogBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CAcModuleResourceOverride resOverride;
	if (CAdUiDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_menu.CreatePopupMenu();
	m_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_1, _T("菜单项 1"));
	m_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_2, _T("菜单项 2"));
	m_menu.AppendMenu(MF_STRING, ID_MENU_ITEM_3, _T("菜单项 3"));

	CRect rect(10, 10, 100, 30);
	pButton = new CButton();
	if (!pButton->Create(_T("菜单"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, 64000))
	{
		AfxMessageBox(_T("按钮创建失败！"));
		delete pButton;
		pButton = nullptr;
		return -1;
	}

	CFont* pFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pButton->SetFont(pFont);
	pButton->EnableWindow(TRUE); // 显式启用按钮
	GetDockingFrame()->SetFocus();
	return 0;
}

void CPolaDialogBar::OnButtonClicked()
{
	CPoint point;
	GetCursorPos(&point);
	m_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}
