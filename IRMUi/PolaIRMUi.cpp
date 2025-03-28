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
#include "PolaIRM.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaIRMUi, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaIRMUi, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SEL_BEAM, &CPolaIRMUi::OnBnClickedButtonSelBeam)
	ON_BN_CLICKED(IDC_BUTTON_IRM, &CPolaIRMUi::OnBnClickedButtonIrm)
	ON_EN_KILLFOCUS(IDC_EDIT_TOP_M_R_NUM, &CPolaIRMUi::OnEnKillfocusEditTopMRNum)
	ON_EN_KILLFOCUS(IDC_EDIT_TOP_M_R_D, &CPolaIRMUi::OnEnKillfocusEditTopMRD)
	ON_EN_KILLFOCUS(IDC_EDIT_BOT_M_R_NUM, &CPolaIRMUi::OnEnKillfocusEditBotMRNum)
	ON_EN_KILLFOCUS(IDC_EDIT_BOT_M_R_D, &CPolaIRMUi::OnEnKillfocusEditBotMRD)
	ON_EN_KILLFOCUS(IDC_EDIT_STIR_D, &CPolaIRMUi::OnEnKillfocusEditStirD)
	ON_EN_KILLFOCUS(IDC_EDIT_STIR_S, &CPolaIRMUi::OnEnKillfocusEditStirS)
	ON_EN_KILLFOCUS(IDC_EDIT_STIR_LIMB_NUM, &CPolaIRMUi::OnEnKillfocusEditStirLimbNum)
	ON_EN_KILLFOCUS(IDC_EDIT_SIDE_NUM, &CPolaIRMUi::OnEnKillfocusEditSideNum)
	ON_EN_KILLFOCUS(IDC_EDIT_SIDE_D, &CPolaIRMUi::OnEnKillfocusEditSideD)
	ON_BN_CLICKED(IDC_RADIO_TOP, &CPolaIRMUi::OnBnClickedRadioTop)
	ON_BN_CLICKED(IDC_RADIO_BOTTOM, &CPolaIRMUi::OnBnClickedRadioBottom)
	ON_BN_CLICKED(IDC_BUTTON_IRM_ADDITION, &CPolaIRMUi::OnBnClickedButtonIrmAddition)
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
	DDX_Control(pDX, IDC_EDIT_B_A_R_NUM, Edit_beam_num);
	DDX_Control(pDX, IDC_EDIT_B_A_R_D, Edit_beam_d);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaIRMUi::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

bool CPolaIRMUi::CheckMainReinforceInfo()
{
	if (!InputValidator<int>::ValidateRebarDiameter(Edit_stir_d, stirrup_d, _T("Rebar diameter")))
		return false;

	if (!InputValidator<int>::ValidateRebarDiameter(Edit_top_m_r_d, top_m_r_d, _T("Rebar diameter")))
		return false;

	if (!InputValidator<int>::ValidateRebarDiameter(Edit_bot_m_r_d, bot_m_r_d, _T("Rebar diameter")))
		return false;
	int max_top_reinforcement = BasicTools::CalculateMaxBars(static_cast<int>(beam_b), top_m_r_d, 45, stirrup_d, 30, 1.5);
	int max_bot_reinforcement = BasicTools::CalculateMaxBars(static_cast<int>(beam_b), bot_m_r_d, 45, stirrup_d, 25, 1);

	if (!InputValidator<int>::Validate(Edit_top_m_r_num, top_m_r_num, _T("Top main reinforcement bar number"), true, 6496, true, 1, max_top_reinforcement))
		return false;

	if (!InputValidator<int>::Validate(Edit_bot_m_r_num, bot_m_r_num, _T("Bottom main reinforcement bar number"), true, 6496, true, 1, max_bot_reinforcement))
		return false;

	int max_stirrup_limbs = (std::max)(top_m_r_num, bot_m_r_num);
	int min_stirrup_limbs = (std::max)(top_m_r_num, bot_m_r_num) / 2;

	if (!InputValidator<int>::Validate(Edit_stir_s, stirrup_s, _T("Stirrup space"), true, 6496, true, 100, 400))
		return false;

	if (!InputValidator<int>::Validate(Edit_stir_limb, stirrup_limb, _T("Stirrup limbs"), true, 6496, true, min_stirrup_limbs, max_stirrup_limbs))
		return false;

	if (!InputValidator<int>::Validate(Edit_side_num, side_num, _T("Side reinforcement bar number"), true, 6496, true, 1, 50))
		return false;

	if (!InputValidator<int>::ValidateRebarDiameter(Edit_side_d, side_d, _T("Rebar diameter")))
		return false;

	return true;
}

bool CPolaIRMUi::CheckAdditionReinforceInfo()
{
	if (!InputValidator<int>::ValidateRebarDiameter(Edit_stir_d, stirrup_d, _T("Rebar diameter")))
		return false;

	if (!InputValidator<int>::ValidateRebarDiameter(Edit_top_m_r_d, top_m_r_d, _T("Rebar diameter")))
		return false;

	if (!InputValidator<int>::ValidateRebarDiameter(Edit_bot_m_r_d, bot_m_r_d, _T("Rebar diameter")))
		return false;
	int max_top_reinforcement = BasicTools::CalculateMaxBars(static_cast<int>(beam_b), top_m_r_d, 45, stirrup_d, 30, 1.5);
	int max_bot_reinforcement = BasicTools::CalculateMaxBars(static_cast<int>(beam_b), bot_m_r_d, 45, stirrup_d, 25, 1);

	if (!InputValidator<int>::Validate(Edit_top_m_r_num, top_m_r_num, _T("Top main reinforcement bar number"), true, 6496, true, 1, max_top_reinforcement))
		return false;

	if (!InputValidator<int>::Validate(Edit_bot_m_r_num, bot_m_r_num, _T("Bottom main reinforcement bar number"), true, 6496, true, 1, max_bot_reinforcement))
		return false;

	int max_stirrup_limbs = (std::max)(top_m_r_num, bot_m_r_num);
	int min_stirrup_limbs = (std::max)(top_m_r_num, bot_m_r_num) / 2;

	if (!InputValidator<int>::Validate(Edit_stir_s, stirrup_s, _T("Stirrup space"), true, 6496, true, 100, 400))
		return false;

	if (!InputValidator<int>::Validate(Edit_stir_limb, stirrup_limb, _T("Stirrup limbs"), true, 6496, true, min_stirrup_limbs, max_stirrup_limbs))
		return false;

	if (!InputValidator<int>::Validate(Edit_side_num, side_num, _T("Side reinforcement bar number"), true, 6496, true, 1, 50))
		return false;

	if (!InputValidator<int>::ValidateRebarDiameter(Edit_side_d, side_d, _T("Rebar diameter")))
		return false;

	if (!InputValidator<int>::Validate(Edit_column_num, column_num, _T("Column end addition reinforcement bar number"), true, 6496, true, 0, 50))
		return false;
	if (!InputValidator<int>::ValidateRebarDiameter(Edit_column_d, column_d, _T("Rebar diameter")))
		return false;

	if (!InputValidator<int>::Validate(Edit_beam_num, beam_num, _T("Column end addition reinforcement bar number"), true, 6496, true, 0, 50))
		return false;
	if (!InputValidator<int>::ValidateRebarDiameter(Edit_beam_d, beam_d, _T("Rebar diameter")))
		return false;
	return true;
}

void CPolaIRMUi::SetDefaultValue()
{
	top_m_r_d = 28;
	bot_m_r_d = 28;
	top_m_r_num = static_cast<int>(beam_b / 100.0);
	bot_m_r_num = top_m_r_num;
	stirrup_d = 12;
	stirrup_s = 100;
	stirrup_limb = static_cast<int>(top_m_r_num / 2.0);
	CString m_r_num;
	m_r_num.Format(_T("%d"), top_m_r_num);

	Edit_top_m_r_d.SetWindowTextW(_T("28"));
	Edit_bot_m_r_d.SetWindowTextW(_T("28"));

	Edit_top_m_r_num.SetWindowTextW(m_r_num);
	Edit_bot_m_r_num.SetWindowTextW(m_r_num);

	Edit_side_d.SetWindowTextW(_T("20"));
	Edit_side_num.SetWindowTextW(_T("6"));

	CString s_r_num;
	s_r_num.Format(_T("%d"), stirrup_limb);
	Edit_stir_d.SetWindowTextW(_T("12"));
	Edit_stir_s.SetWindowTextW(_T("100"));
	Edit_stir_limb.SetWindowTextW(s_r_num);
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
	insert_point = beam->getHorizontalMidPoint() + AcGeVector3d(0, beam_b / 4.0, 0);

	AcGePoint3dArray temp_array;
	if (BeamTools::GetAllPillarCenterInBeam(beam, temp_array))
	{
		column_end_addition = temp_array;
	}

	if (BeamTools::GetBeamSegmentMidPoint(beam, temp_array))
	{
		beam_mid_addition = temp_array;
	}

	Edit_beam_name.SetWindowTextW(beam_name);
	Edit_beam_seg_num.SetWindowTextW(beam_segment_num);
	Edit_beam_b.SetWindowTextW(beam_width);
	Edit_beam_h.SetWindowTextW(beam_height);
	CompleteEditorCommand();
	SetDefaultValue();
}

void CPolaIRMUi::OnBnClickedButtonIrm()
{
	if (CheckMainReinforceInfo())
	{
		irm.setBeamInfo(beam_sn, beam_seg_num, beam_b, beam_h);
		irm.setStirrupReinforcementInfo(stirrup_d, stirrup_s, stirrup_limb);
		irm.setMainReinforcementInfo(top_m_r_num, top_m_r_d, bot_m_r_num, bot_m_r_d);
		irm.setSideReinforcementInfo(side_num, side_d);
		irm.setInsertPoint(insert_point);
		EditEntity::SetLayer(irm.DrawPolaIrmMain(), _T("POLA_IRM_MARK"));
		MessageBox(_T("Main IRM info add success!"));
	}
}

void CPolaIRMUi::OnEnKillfocusEditTopMRNum()
{
	InputValidator<int>::Validate(Edit_top_m_r_num, top_m_r_d, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditTopMRD()
{
	InputValidator<int>::ValidateRebarDiameter(Edit_top_m_r_d, top_m_r_d, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditBotMRNum()
{
	InputValidator<int>::Validate(Edit_bot_m_r_num, bot_m_r_d, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditBotMRD()
{
	InputValidator<int>::ValidateRebarDiameter(Edit_bot_m_r_d, bot_m_r_d, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditStirD()
{
	InputValidator<int>::ValidateRebarDiameter(Edit_stir_d, stirrup_d, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditStirS()
{
	InputValidator<int>::Validate(Edit_stir_s, stirrup_s, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditStirLimbNum()
{
	InputValidator<int>::Validate(Edit_stir_limb, stirrup_limb, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditSideNum()
{
	InputValidator<int>::Validate(Edit_side_num, side_num, _T(" "), false);
}

void CPolaIRMUi::OnEnKillfocusEditSideD()
{
	InputValidator<int>::ValidateRebarDiameter(Edit_side_d, side_d, _T(" "), false);
}

void CPolaIRMUi::OnBnClickedRadioTop()
{
	direction_flag = UP;
}

void CPolaIRMUi::OnBnClickedRadioBottom()
{
	direction_flag = DOWN;
}

BOOL CPolaIRMUi::OnInitDialog()
{
	CAdUiBaseDialog::OnInitDialog();
	CheckRadioButton(IDC_RADIO_TOP, IDC_RADIO_BOTTOM, IDC_RADIO_TOP);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPolaIRMUi::OnBnClickedButtonIrmAddition()
{
	int selected_radio_id = -1;
	selected_radio_id = GetCheckedRadioButton(IDC_RADIO_TOP, IDC_RADIO_BOTTOM);

	AcGeVector3d offset_vector_column;
	AcGeVector3d offset_vecror_beam;
	switch (selected_radio_id)
	{
	case IDC_RADIO_TOP:
		offset_vector_column = AcGeVector3d(0, beam_b / 2.0 + 200, 0);
		offset_vecror_beam = AcGeVector3d(0, -beam_b / 2.0 - 200 - 350, 0);
		break;
	case IDC_RADIO_BOTTOM:
		offset_vector_column = AcGeVector3d(0, -beam_b / 2.0 - 200 - 350, 0);
		offset_vecror_beam = AcGeVector3d(0, beam_b / 2.0 + 200, 0);
	default:
		break;
	}
	if (CheckAdditionReinforceInfo())
	{
		AcDbObjectIdArray irm_text;
		irm.setStirrupReinforcementInfo(stirrup_d, stirrup_s, stirrup_limb);
		irm.setColumnEndAdditionReinforcementInfo(column_num, column_d);
		irm.setColumnAdditionInsertPoints(column_end_addition);
		irm_text = irm.DrawPolaIrmColumnAddition(offset_vector_column);
		EditEntity::SetLayer(irm_text, _T("POLA_IRM_MARK"));
		EditEntity::SetTextHorzMode(irm_text, AcDb::kTextCenter);

		if (beam_num == 0 || beam_d == 0)
		{
			MessageBox(_T("Column end addititon info add success!"));
			return;
		}
		irm_text.removeAll();
		irm.setBeamMidAdditionReinforcementInfo(beam_num, beam_d);
		irm.setBeamMidInsertPoints(beam_mid_addition);
		irm_text = irm.DrawPolaIrmBeamAddition(offset_vecror_beam);
		EditEntity::SetLayer(irm_text, _T("POLA_IRM_MARK"));
		EditEntity::SetTextHorzMode(irm_text, AcDb::kTextCenter);
		MessageBox(_T("Beam mid and Column end addition info add success!"));
	}
}
