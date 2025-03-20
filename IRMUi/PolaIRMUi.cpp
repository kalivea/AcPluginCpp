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
//----- PolaIRMUi.cpp : Implementation of CPolaIRMUi
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaIRMUi.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaIRMUi, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaIRMUi, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SEL_BEAM, &CPolaIRMUi::OnBnClickedButtonSelBeam)
	ON_BN_CLICKED(IDC_BUTTON_IRM, &CPolaIRMUi::OnBnClickedButtonIrm)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaIRMUi::CPolaIRMUi(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog(CPolaIRMUi::IDD, pParent, hInstance) {

}


//-----------------------------------------------------------------------------
void CPolaIRMUi::DoDataExchange(CDataExchange* pDX) {
	CAdUiBaseDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_BEAM_INFO, Edit_beam_name);
	DDX_Control(pDX, IDC_EDIT_BEAM_SGE_NUM, Edit_beam_seg_num);
	DDX_Control(pDX, IDC_EDIT_BEAM_B, Edit_beam_b);
	DDX_Control(pDX, IDC_EDIT_BEAM_H, Edit_beam_h);
	DDX_Control(pDX, IDC_EDIT_TOP_M_R_NUM, Edit_top_m_r_num);
	DDX_Control(pDX, IDC_EDIT_TOP_M_R_D, Edit_top_m_r_d);
	DDX_Control(pDX, IDC_EDIT_BOT_M_R_NUM, Edit_bot_m_r_num);
	DDX_Control(pDX, IDC_EDIT_BOT_M_R_D, Edit_bot_m_r_d);
	DDX_Control(pDX, IDC_EDIT_STIR_LIMB_NUM, Edit_stir_limb);
	DDX_Control(pDX, IDC_EDIT_STIR_D, Edit_stir_d);
	DDX_Control(pDX, IDC_EDIT_STIR_S, Edit_stir_s);
	DDX_Control(pDX, IDC_EDIT_SIDE_NUM, Edit_side_num);
	DDX_Control(pDX, IDC_EDIT_SIDE_D, Edit_side_d);
	DDX_Control(pDX, IDC_EDIT_C_A_R_NUM, Edit_column_num);
	DDX_Control(pDX, IDC_EDIT_C_A_R_D, Edit_column_d);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaIRMUi::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

bool CPolaIRMUi::CheckReinforceInfo()
{
	int max_stirrup_limbs = (std::max)(top_m_r_num, bot_m_r_num);
	int min_stirrup_limbs = (std::max)(top_m_r_num, bot_m_r_num) / 2;
	bool stirrup_check =
		InputValidator<int>::Validate(Edit_stir_d, stirrup_d, _T("Stirrup diameter"), true, 6496, true, 4, 40) &&
		InputValidator<int>::Validate(Edit_stir_s, stirrup_s, _T("Stirrup space"), true, 6496, true, 100, 400) &&
		InputValidator<int>::Validate(Edit_stir_limb, stirrup_limb, _T("Stirrup spaceing"), true, 6496, true, min_stirrup_limbs, max_stirrup_limbs);

	int max_top_reinforcement = BasicTools::CalculateMaxBars(static_cast<int>(beam_b), top_m_r_d, 45, stirrup_d, 30, 1.5);
	int max_bot_reinforcement = BasicTools::CalculateMaxBars(static_cast<int>(beam_b), bot_m_r_d, 45, stirrup_d, 25, 1);
	bool main_reinforce_check =
		InputValidator<int>::Validate(Edit_top_m_r_num, top_m_r_num, _T("Top main reinforcement bar number"), true, 6496, true, 1, max_top_reinforcement) &&
		InputValidator<int>::Validate(Edit_bot_m_r_num, bot_m_r_num, _T("Bottom main reinforcement bar number"), true, 6496, true, 1, max_bot_reinforcement) &&
		InputValidator<int>::Validate(Edit_top_m_r_d, top_m_r_d, _T("Top main reinforcement bar diameter"), true, 6496, true, 4, 40) &&
		InputValidator<int>::Validate(Edit_bot_m_r_d, bot_m_r_d, _T("Bottom main reinforcement bar diameter"), true, 6496, true, 4, 40);

	bool side_reinforce_check =
		InputValidator<int>::Validate(Edit_side_num, side_num, _T("Side reinforcement bar number"), true, 6496, true, 1, 50) &&
		InputValidator<int>::Validate(Edit_side_d, side_d, _T("Side reinforcement bar diameter"), true, 6496, true, 4, 40);

	bool column_addition_check =
		InputValidator<int>::Validate(Edit_column_num, column_num, _T("Column end addition reinforcement bar number"), true, 6496, true, 0, 50) &&
		InputValidator<int>::Validate(Edit_column_d, column_d, _T("Column end addition reinforcement bar diameter"), true, 6496, true, 4, 40);

	return false;
}

void CPolaIRMUi::OnBnClickedButtonSelBeam()
{
	BeginEditorCommand();
	AcDbObjectPointer<CPolaCustomBeam> beam;
	AcDbObjectIdArray beam_id;
	while (true)
	{
		if (!SelectEntitys::PickEntitys(_T("Pick one beam\n"), CPolaCustomBeam::desc(), beam_id))
		{
			CompleteEditorCommand();
			return;
		}

		if (beam_id.length() == 1)
		{
			break;
		}
		else
		{
			acutPrintf(_T("Please pick exactly one beam entity.\n"));
		}
	}
	beam.open(beam_id.at(0));
	beam_b = beam->getBeamWidth();
	beam_h = beam->getBeamHeight();
	beam_sn = beam->getBeamProperty();
	beam_seg_num = beam->getBeamSegmentNum();
	CString beam_name;
	beam_name.Format(_T("KL%d"), beam_sn);
	CString beam_width;
	beam_width.Format(_T("%d"), static_cast<int>(beam_b));
	CString beam_height;
	beam_height.Format(_T("%d"), static_cast<int>(beam_h));
	CString beam_segment_num;
	beam_segment_num.Format(_T("(%d)"), beam_seg_num);
	insert_point = beam->getHorizontalMidPoint();

	Edit_beam_name.SetWindowTextW(beam_name);
	Edit_beam_seg_num.SetWindowTextW(beam_segment_num);
	Edit_beam_b.SetWindowTextW(beam_width);
	Edit_beam_h.SetWindowTextW(beam_height);
	CompleteEditorCommand();
}

void CPolaIRMUi::OnBnClickedButtonIrm()
{
	CheckReinforceInfo();
}
