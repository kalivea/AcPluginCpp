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
void SendCommand(const TCHAR* szCommand)
{
	COPYDATASTRUCT msg;
	msg.dwData = static_cast<DWORD> (1);
	msg.cbData = static_cast<DWORD> ((_tcslen(szCommand) + 1) * sizeof(TCHAR));
	msg.lpData = const_cast<TCHAR*> (szCommand);

	acedGetAcadFrame()->SendMessage(WM_COPYDATA, reinterpret_cast<WPARAM>(adsw_acadMainWnd()), reinterpret_cast<LPARAM>(&msg));
	acedGetAcadDockCmdLine()->SetFocus();
}
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaDialogBar, CAdUiDialogBar)

BEGIN_MESSAGE_MAP(CPolaDialogBar, CAdUiDialogBar)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND_RANGE(POLA_MENU_START, POLA_MENU_START + 100, OnMenuSelect)
	ON_BN_CLICKED(IDC_MY_BUTTON, &CPolaDialogBar::OnButtonClicked)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaDialogBar::CPolaDialogBar(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiDialogBar(/*CPolaDialogBar::IDD, pParent, hInstance*/) {
	m_bIsShow = TRUE;
	m_nAlignment = 0;
	m_bOnce = TRUE;
	Load();
}

CPolaDialogBar::~CPolaDialogBar()
{
	Save();
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

void CPolaDialogBar::Save()
{
	CString config_path = _T("C:\\Users\\Polaris\\Documents\\arxini\\config.ini");
	SHCreateDirectoryEx(NULL, config_path.Left(config_path.ReverseFind('\\')), NULL);

	CString temp_alignment_mode;
	temp_alignment_mode.Format(_T("%d"), m_nAlignment);
	BOOL temp_show = m_bIsShow;

	WritePrivateProfileString(_T("Settings"), _T("alignment"), temp_alignment_mode, config_path);
	WritePrivateProfileString(_T("Settings"), _T("show"), temp_show ? _T("1") : _T("0"), config_path);
}

void CPolaDialogBar::Load()
{
	CString config_path = _T("C:\\Users\\Polaris\\Documents\\arxini\\config.ini");
	if (GetFileAttributes(config_path) == INVALID_FILE_ATTRIBUTES)
		return;
	m_nAlignment = GetPrivateProfileIntW(_T("Settings"), _T("alignment"), 0, config_path);

	BOOL bTemp = GetPrivateProfileIntW(_T("Settings"), _T("show"), 0, config_path);
	m_bIsShow = (bTemp != 0);
}

//-----------------------------------------------------------------------------
void CPolaDialogBar::DoDataExchange(CDataExchange* pDX) {
	CAdUiDialogBar::DoDataExchange(pDX);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaDialogBar::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

void CPolaDialogBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: 在此添加专用代码和/或调用基类
	CAdUiDialogBar::OnUpdateCmdUI(pTarget, FALSE);
}

void CPolaDialogBar::OnSetFrameFocus()
{
	GetDockingFrame()->SetFocus();
}

void CPolaDialogBar::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bIsShow = FALSE;
	Save();
	CAdUiDialogBar::OnClose();
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

void CPolaDialogBar::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
	if ((GetBarStyle() & CBRS_ALIGN_ANY) == CBRS_ALIGN_BOTTOM)
		m_nAlignment = 1;
	else
		m_nAlignment = 0;
	Save();
}

void CPolaDialogBar::OnMenuSelect(UINT nID)
{
	int i = nID - POLA_MENU_START - 1;

}

int CPolaDialogBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAdUiDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CPolaDialogBar::OnButtonClicked()
{
	/*CPolaMenu cassMenu;
	int nStartID = 3000;
	CStringArray saMenu;
	CPolaMenu::ReadMenuConfigFromFile(_T("C:\\Users\\Polaris\\Documents\\arxini\\menu.cfg"), saMenu);
	cassMenu.Set(saMenu, nStartID);

	CRect rect;
	GetDlgItem(IDC_MY_BUTTON)->GetWindowRect(&rect);
	cassMenu.m_menu.TrackPopupMenu(
		TPM_LEFTALIGN | TPM_TOPALIGN,
		rect.left,
		rect.bottom,
		this
	);*/
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);

	CMenu* pSubMenu = menu.GetSubMenu(0);
	if (!pSubMenu) return;

	CRect rect;
	GetDlgItem(IDC_MY_BUTTON)->GetWindowRect(&rect);

	pSubMenu->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_TOPALIGN,
		rect.left,
		rect.bottom,
		this
	);

}
