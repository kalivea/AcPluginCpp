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
//----- PolaDialogBar.h : Declaration of the CPolaDialogBar
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "resource.h"
#include "CPolaMenu.h"
//-----------------------------------------------------------------------------
class CPolaDialogBar : public CAdUiDialogBar {
	DECLARE_DYNAMIC(CPolaDialogBar)

public:
	CPolaDialogBar(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);
	virtual ~CPolaDialogBar();
	BOOL Create(CFrameWnd* pParentWnd, CString csName);
	void Save();
	void Load();

	enum { IDD = IDD_POLADIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	afx_msg void OnSetFrameFocus();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);
	afx_msg void OnMenuSelect(UINT nID);
public:

	UINT m_bIsShow;
	UINT m_nAlignment;
	CString m_csPath;
	bool m_bOnce;
	CPolaMenu m_polaMnu;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonClicked();
};
