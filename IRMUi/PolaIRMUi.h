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
//----- PolaIRMUi.h : Declaration of the CPolaIRMUi
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "resource.h"
#include "PolaIRM.h"
//-----------------------------------------------------------------------------
class CPolaIRMUi : public CAdUiBaseDialog {
	DECLARE_DYNAMIC(CPolaIRMUi)

public:
	CPolaIRMUi(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);

	enum { IDD = IDD_POLAIRMUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:

	CEdit Edit_beam_name;
	int beam_sn;
	CEdit Edit_beam_seg_num;
	int beam_seg_num = 6496;
	CEdit Edit_beam_b;
	double beam_b = 6496;
	CEdit Edit_beam_h;
	double beam_h = 6496;
	AcGePoint3d insert_point;
	AcGePoint3dArray column_end_addition;
	AcGePoint3dArray beam_mid_addition;

	CEdit Edit_top_m_r_num;
	int top_m_r_num = 6496;
	CEdit Edit_top_m_r_d;
	int top_m_r_d = 6496;
	CEdit Edit_bot_m_r_num;
	int bot_m_r_num = 6496;
	CEdit Edit_bot_m_r_d;
	int bot_m_r_d = 6496;

	CEdit Edit_stir_d;
	int stirrup_d = 6496;
	CEdit Edit_stir_s;
	int stirrup_s = 6496;
	CEdit Edit_stir_limb;
	int stirrup_limb = 6496;

	CEdit Edit_side_num;
	int side_num = 6496;
	CEdit Edit_side_d;
	int side_d = 6496;
	CEdit Edit_column_num;
	int column_num = 6496;
	CEdit Edit_column_d;
	int column_d = 6496;

	CEdit Edit_beam_num;
	int beam_num = 6496;
	CEdit Edit_beam_d;
	int beam_d = 6496;

	PolaIRM irm;
	PolaIRM::Type beam_type;
	bool CheckMainReinforceInfo();
	bool CheckAdditionReinforceInfo();
	void SetDefaultValue();
	afx_msg void OnBnClickedButtonSelBeam();
	afx_msg void OnBnClickedButtonIrm();
	afx_msg void OnEnKillfocusEditTopMRNum();
	afx_msg void OnEnKillfocusEditTopMRD();
	afx_msg void OnEnKillfocusEditBotMRNum();
	afx_msg void OnEnKillfocusEditBotMRD();
	afx_msg void OnEnKillfocusEditStirD();
	afx_msg void OnEnKillfocusEditStirS();
	afx_msg void OnEnKillfocusEditStirLimbNum();
	afx_msg void OnEnKillfocusEditSideNum();
	afx_msg void OnEnKillfocusEditSideD();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonIrmAddition();
};
