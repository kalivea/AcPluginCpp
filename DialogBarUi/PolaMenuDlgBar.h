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
//----- PolaMenuDlgBar.h : Declaration of the CPolaMenuDlgBar
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "resource.h"
#include "PolaMenu.h"
//-----------------------------------------------------------------------------
class CPolaMenuDlgBar : public CAdUiDialogBar {
	DECLARE_DYNAMIC(CPolaMenuDlgBar)

public:
	CPolaMenuDlgBar(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);
	virtual ~CPolaMenuDlgBar();
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	afx_msg void OnSetFrameFocus();
	afx_msg void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);
	afx_msg void OnClose();
	afx_msg void OnBtnCassMnu();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuSelect(UINT nID);

	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(CFrameWnd* parent_wnd, CString name);

	bool isShow;
	Adesk::UInt32 alignment_mode;
	CString path;
	bool isOnce;
	PolaMenu menu_;
	void Save();
	void ShowBar(bool show = true);
protected:
	void Load();
};
