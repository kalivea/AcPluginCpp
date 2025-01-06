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
//----- PolaCustomUi.h : Declaration of the CPolaCustomUi
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "resource.h"
#include <acuiEdit.h>

//-----------------------------------------------------------------------------
class CPolaCustomUi : public CAdUiBaseDialog {
	DECLARE_DYNAMIC(CPolaCustomUi)

public:
	CPolaCustomUi(CWnd* pParent = NULL, HINSTANCE hInstance = NULL);

	enum { IDD = IDD_POLACUSTOMUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CAcUiEdit Edit_b;
	CAcUiEdit Edit_h;
	CAcUiEdit Edit_viewable;
	CAcUiEdit Edit_shape;
	CAcUiEdit Edit_x;
	CAcUiEdit Edit_y;

	double b;
	double h;
	int viewable;
	int shape;
	double x;
	double y;

	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	void ScalePattern(CRect& rect, CPoint vertex_point[4]) const;
};
