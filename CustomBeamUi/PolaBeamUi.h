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
//----- PolaBeamUi.h : Declaration of the CPolaBeamUi
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "resource.h"
#include "InputValidator.h"
//-----------------------------------------------------------------------------
class CPolaBeamUi : public CAdUiBaseDialog {
	DECLARE_DYNAMIC(CPolaBeamUi)

public:
	CPolaBeamUi(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);

	enum { IDD = IDD_BEAM_MODAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	CEdit Edit_Beam_Sn_;
	int beam_Sn = 6496;
	CEdit Edit_Beam_b_;
	double beam_b = 6496;
	CEdit Edit_Beam_h_;
	double beam_h = 6496;
	CEdit Edit_Beam_Slab;
	double slab_thickness = 6496;
	CEdit Edit_Beam_Slab_offset;
	double offset_length = 6496;
	afx_msg void OnBnClickedButtonPickpillar();
	afx_msg void OnBnClickedButtonPickoffset();
	afx_msg void OnBnClickedButtonEditviewable();
	afx_msg void OnBnClickedButtonAddvertex();
	afx_msg void OnBnClickedButtonAddjoint();
	afx_msg void OnEnKillfocusEditBeamSn();
	afx_msg void OnEnKillfocusEditBeamB();
	afx_msg void OnEnKillfocusEditBeamH();
	afx_msg void OnEnKillfocusEditBeamSlab();
	afx_msg void OnEnKillfocusEditBeamSlaboffset();
	afx_msg void OnBnClickedButtonAddinfo();
};
