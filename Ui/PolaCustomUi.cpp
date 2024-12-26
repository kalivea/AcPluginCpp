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
//----- PolaCustomUi.cpp : Implementation of CPolaCustomUi
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaCustomUi.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaCustomUi, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaCustomUi, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDOK, &CPolaCustomUi::OnBnClickedOk)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaCustomUi::CPolaCustomUi(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog(CPolaCustomUi::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CPolaCustomUi::DoDataExchange(CDataExchange* pDX) {
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit_b);
	DDX_Control(pDX, IDC_EDIT2, Edit_h);
	DDX_Control(pDX, IDC_EDIT3, Edit_viewable);
	DDX_Control(pDX, IDC_EDIT4, Edit_shape);
	DDX_Control(pDX, IDC_EDIT5, Edit_x);
	DDX_Control(pDX, IDC_EDIT6, Edit_y);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaCustomUi::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

void CPolaCustomUi::OnBnClickedOk()
{
	CString temp;
	Edit_b.GetWindowTextW(temp);
	b = _wtof(temp);
	Edit_h.GetWindowTextW(temp);
	h = _wtof(temp);

	Edit_viewable.GetWindowTextW(temp);
	viewable = _wtoi(temp);

	Edit_shape.GetWindowTextW(temp);
	shape = _wtoi(temp);
	Edit_x.GetWindowTextW(temp);
	x = _wtof(temp);
	Edit_y.GetWindowTextW(temp);
	y = _wtoi(temp);
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CPolaCustomUi::PreviewPillar()
{
	CDC* dc = new CDC();
	HDC hdc = ::GetDC(GetDlgItem(IDC_STATIC)->m_hWnd);
}

