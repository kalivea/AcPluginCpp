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
//----- OptionSheet.cpp : Implementation of COptionSheet
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "OptionSheet.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (COptionSheet, CAcUiTabMainDialog)

BEGIN_MESSAGE_MAP(COptionSheet, CAcUiTabMainDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
COptionSheet::COptionSheet (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiTabMainDialog (COptionSheet::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void COptionSheet::DoDataExchange (CDataExchange *pDX) {
	CAcUiTabMainDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, Tab_option_tab);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT COptionSheet::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL COptionSheet::OnInitDialog()
{
	CAcUiTabMainDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetAcadTabPointer(&Tab_option_tab);
	Tab_option_tab.AddTab(0, _T("明挖"), CCutAndCover::IDD, &cut_and_cover_page);
	Tab_option_tab.AddTab(1, _T("盖挖"), CCoverAndCut::IDD, &cover_and_cut_page);
	Tab_option_tab.AddTab(2, _T("暗挖"), CUnderCutting::IDD, &under_cutting_page);
	Tab_option_tab.AddTab(3, _T("盾构"), CShielding::IDD, &shielding_page);

	Tab_option_tab.SetActiveTab(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
