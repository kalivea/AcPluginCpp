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
//----- PolaPillarUi.h : Declaration of the CPolaPillarUi
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "resource.h"
//-----------------------------------------------------------------------------

enum ShapeType { CIRCLE, RECTANGLE };
enum LineStyle { DASHED, SOLID };

class CPolaPillarUi : public CAdUiBaseDialog {
	DECLARE_DYNAMIC(CPolaPillarUi)

public:
	CPolaPillarUi(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);

	enum { IDD = IDD_POLAPILLARUI };
public:
	void ScalePattern(ShapeType shape, double d1, double d2 = 0, LineStyle style = SOLID);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioRect();
	afx_msg void OnBnClickedRadioCirc();
	CEdit Edit_Sn_;
	int beam_sn;
	CEdit Edit_Grade;
	int beam_conc_grade;
	CEdit Edit_Pipe_D_;
	double pipe_d = 0;
	CEdit Edit_Pipe_T_;
	double pipe_t = 0;
	CEdit Edit_Pillar_D_;
	double pillar_d = 0;
	CEdit Edit_Pillar_H_;
	double pillar_h = 0;

	CRect rc_draw_rect;
	ShapeType shape_type;
	LineStyle line_style;
	BOOL isDataReady = false;

	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusEditSn();
	afx_msg void OnEnKillfocusEditConcGrade();
	afx_msg void OnEnKillfocusEditPipeD();
	afx_msg void OnEnKillfocusEditPipeT();
	afx_msg void OnEnKillfocusEditD();
	afx_msg void OnEnKillfocusEditH();
	afx_msg void OnBnClickedButtonPreview();
	afx_msg void OnPaint();
};
