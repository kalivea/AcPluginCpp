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
//----- PolaMenuDlgBar.cpp : Implementation of CPolaMenuDlgBar
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaMenuDlgBar.h"
#include "PolaMenu.h"

extern PolaMenu pMenu;
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaMenuDlgBar, CAdUiDialogBar)

BEGIN_MESSAGE_MAP(CPolaMenuDlgBar, CAdUiDialogBar)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaMenuDlgBar::CPolaMenuDlgBar(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiDialogBar(/*CPolaMenuDlgBar::IDD, pParent, hInstance*/) {
}

CPolaMenuDlgBar::~CPolaMenuDlgBar()
{
	Save();
}

//-----------------------------------------------------------------------------
void CPolaMenuDlgBar::DoDataExchange(CDataExchange* pDX) {
	CAdUiDialogBar::DoDataExchange(pDX);
}

void CPolaMenuDlgBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CDialogBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaMenuDlgBar::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

void CPolaMenuDlgBar::OnSetFrameFocus()
{
	GetDockingFrame()->SetFocus();
}

void CPolaMenuDlgBar::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
	if ((GetBarStyle() & CBRS_ALIGN_ANY) == CBRS_ALIGN_BOTTOM)
		alignment_mode = 1;
	else
		alignment_mode = 0;
	Save();
}

void CPolaMenuDlgBar::OnClose()
{
	isShow = FALSE;
	Save();
	CAdUiDialogBar::OnClose();
}

void CPolaMenuDlgBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (isOnce)
	{
		isOnce = FALSE;

		CStringArray saMenu;
		CStdioFile stdF;
		if (stdF.Open(path, CFile::modeRead))
		{
			CString csStr;
			while (stdF.ReadString(csStr))
			{
				int nPos = csStr.Find(_T("["), 0);
				if (-1 == nPos)
				{
					saMenu.Add(csStr);
				}
				else
				{
					saMenu.Add(csStr.Right(csStr.GetLength() - nPos));
				}
			}
			stdF.Close();
		}

		int nID = IMENU_ITEM_ID;
		menu_.Set(saMenu, nID);

		CMenu m_menu; // 成员变量（确保生命周期）
		m_menu.CreateMenu(); // 创建空的主菜单栏

		// 使用示例：
		SetMenu(&menu_.menu_);
	}
}

void CPolaMenuDlgBar::OnSize(UINT nType, int cx, int cy)
{
	CAdUiDialogBar::OnSize(nType, cx, cy);
}

BOOL CPolaMenuDlgBar::Create(CFrameWnd* parent_wnd, CString name)
{
	CAcModuleResourceOverride res;

	if (!CAdUiDialogBar::Create(parent_wnd, IDD_DIALOG1, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM, AFX_IDW_CONTROLBAR_LAST))
		return FALSE;
	SetBorders();
	SetWindowText(name);
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);

	if (alignment_mode == 0)
		GetDockingFrame()->DockControlBar(this, AFX_IDW_DOCKBAR_TOP);
	else
		GetDockingFrame()->DockControlBar(this, AFX_IDW_DOCKBAR_BOTTOM);

	GetDockingFrame()->ShowControlBar(this, isShow, FALSE);
	GetDockingFrame()->RecalcLayout();

	return TRUE;
}

void CPolaMenuDlgBar::Save()
{
	CString config_path = _T("C:\\Users\\Polaris\\Documents\\arxini\\config.ini");
	SHCreateDirectoryEx(NULL, config_path.Left(config_path.ReverseFind('\\')), NULL);

	CString temp_alignment_mode;
	temp_alignment_mode.Format(_T("%d"), alignment_mode);
	BOOL temp_show = isShow;

	WritePrivateProfileString(_T("Settings"), _T("alignment"), temp_alignment_mode, config_path);
	WritePrivateProfileString(_T("Settings"), _T("show"), temp_show ? _T("1") : _T("0"), config_path);
}

void CPolaMenuDlgBar::ShowBar(bool show)
{
	GetDockingFrame()->ShowControlBar(this, show, FALSE);
	GetDockingFrame()->RecalcLayout();
	isShow = show;
	Save();
}

void CPolaMenuDlgBar::Load()
{
	CString config_path = _T("C:\\Users\\Polaris\\Documents\\arxini\\config.ini");
	if (GetFileAttributes(config_path) == INVALID_FILE_ATTRIBUTES)
		return;
	alignment_mode = GetPrivateProfileIntW(_T("Settings"), _T("alignment"), 0, config_path);

	BOOL bTemp = GetPrivateProfileIntW(_T("Settings"), _T("show"), 0, config_path);
	isShow = (bTemp != 0);
}
