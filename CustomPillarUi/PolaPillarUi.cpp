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
//----- PolaPillarUi.cpp : Implementation of CPolaPillarUi
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaPillarUi.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CPolaPillarUi, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaPillarUi, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_RADIO_RECT, &CPolaPillarUi::OnBnClickedRadioRect)
	ON_BN_CLICKED(IDC_RADIO_CIRC, &CPolaPillarUi::OnBnClickedRadioCirc)
	ON_EN_KILLFOCUS(IDC_EDIT_SN, &CPolaPillarUi::OnEnKillfocusEditSn)
	ON_EN_KILLFOCUS(IDC_EDIT_CONC_GRADE, &CPolaPillarUi::OnEnKillfocusEditConcGrade)
	ON_EN_KILLFOCUS(IDC_EDIT_PIPE_D, &CPolaPillarUi::OnEnKillfocusEditPipeD)
	ON_EN_KILLFOCUS(IDC_EDIT_PIPE_T, &CPolaPillarUi::OnEnKillfocusEditPipeT)
	ON_EN_KILLFOCUS(IDC_EDIT_D, &CPolaPillarUi::OnEnKillfocusEditD)
	ON_EN_KILLFOCUS(IDC_EDIT_H, &CPolaPillarUi::OnEnKillfocusEditH)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaPillarUi::CPolaPillarUi (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog (CPolaPillarUi::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CPolaPillarUi::DoDataExchange (CDataExchange *pDX) {
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SN, Edit_Sn_);
	DDX_Control(pDX, IDC_EDIT_CONC_GRADE, Edit_Grade);
	DDX_Control(pDX, IDC_EDIT_PIPE_D, Edit_Pipe_D_);
	DDX_Control(pDX, IDC_EDIT_PIPE_T, Edit_Pipe_T_);
	DDX_Control(pDX, IDC_EDIT_D, Edit_Pillar_D_);
	DDX_Control(pDX, IDC_EDIT_H, Edit_Pillar_H_);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaPillarUi::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CPolaPillarUi::OnBnClickedRadioRect()
{
	int selected_id = GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC);
	bool enable_edit = (selected_id == IDC_RADIO_RECT);
	GetDlgItem(IDC_EDIT_D)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_H)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_D)->EnableWindow(!enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_T)->EnableWindow(!enable_edit);
}

void CPolaPillarUi::OnBnClickedRadioCirc()
{
	int selected_id = GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC);
	bool enable_edit = (selected_id == IDC_RADIO_CIRC);

	GetDlgItem(IDC_EDIT_CONC_GRADE)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_D)->EnableWindow(!enable_edit);
	GetDlgItem(IDC_EDIT_H)->EnableWindow(!enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_D)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_T)->EnableWindow(enable_edit);
}

BOOL CPolaPillarUi::OnInitDialog()
{
	CAdUiBaseDialog::OnInitDialog();
	CheckRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC, IDC_RADIO_RECT);
	CheckRadioButton(IDC_RADIO_SLINE, IDC_RADIO_DLINE, IDC_RADIO_SLINE);
	bool enable_edit = true;
	GetDlgItem(IDC_EDIT_D)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_H)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_D)->EnableWindow(!enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_T)->EnableWindow(!enable_edit);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPolaPillarUi::OnEnKillfocusEditSn()
{
	CString temp;
	Edit_Sn_.GetWindowTextW(temp);
	beam_sn = _wtoi(temp);
}

void CPolaPillarUi::OnEnKillfocusEditConcGrade()
{
	CString temp;
	Edit_Grade.GetWindowTextW(temp);
	beam_conc_grade = _wtoi(temp);
}

void CPolaPillarUi::OnEnKillfocusEditPipeD()
{
	CString temp;
	if (GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC) == IDC_RADIO_RECT)
	{
		SetDlgItemText(IDC_EDIT_PIPE_D, _T(""));
	}
	else
	{
		Edit_Pipe_D_.GetWindowTextW(temp);
		pipe_d = _wtof(temp);
		pillar_d = pipe_d;
	}
}

void CPolaPillarUi::OnEnKillfocusEditPipeT()
{
	CString temp;
	if (GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC) == IDC_RADIO_RECT)
	{
		SetDlgItemText(IDC_EDIT_PIPE_T, _T(""));
	}
	else
	{
		Edit_Pipe_T_.GetWindowTextW(temp);
		pipe_t = _wtof(temp);
	}
}

void CPolaPillarUi::OnEnKillfocusEditD()
{
	CString temp;
	if (GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC) == IDC_RADIO_CIRC)
	{
		SetDlgItemText(IDC_EDIT_D, _T(""));
	}
	else
	{
		Edit_Pillar_D_.GetWindowTextW(temp);
		pillar_d = _wtof(temp);
	}
}

void CPolaPillarUi::OnEnKillfocusEditH()
{
	CString temp;
	if (GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC) == IDC_RADIO_CIRC)
	{
		SetDlgItemText(IDC_EDIT_H, _T(""));
	}
	else
	{
		Edit_Pillar_H_.GetWindowTextW(temp);
		pillar_h = _wtof(temp);
	}
}
