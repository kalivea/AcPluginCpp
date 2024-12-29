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

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CPolaCustomUi::OnBnClickedButton1)
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

	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CPolaCustomUi::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CAdUiBaseDialog::OnPaint() for painting messages

	CDC* pdc = GetDlgItem(IDC_STATIC_PIC)->GetDC();
	CDC mem_dc;
	mem_dc.CreateCompatibleDC(pdc);
	CRect rect;
	GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);
	CBitmap bit_map;
	bit_map.CreateCompatibleBitmap(&mem_dc, rect.Width(), rect.Height());
	mem_dc.SelectObject(&bit_map);
	mem_dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));
	CPen pen1(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen2(PS_DASH, 1, RGB(0, 0, 0));
	CPoint vertex_point[4];

	if (shape == 1)
	{
		if (viewable == 0)
		{
			mem_dc.SelectObject(&pen2);
			ScalePattern(rect, vertex_point);
			mem_dc.Rectangle(vertex_point[0].x, vertex_point[0].y, vertex_point[2].x, vertex_point[2].y);

			mem_dc.MoveTo(vertex_point[0].x, vertex_point[0].y);
			mem_dc.LineTo(vertex_point[2].x, vertex_point[2].y);

			mem_dc.MoveTo(vertex_point[1].x, vertex_point[1].y);
			mem_dc.LineTo(vertex_point[3].x, vertex_point[3].y);
		}
		else if ((viewable == 1))
		{
			mem_dc.SelectObject(&pen1);
			ScalePattern(rect, vertex_point);
			mem_dc.Rectangle(vertex_point[0].x, vertex_point[0].y, vertex_point[2].x, vertex_point[2].y);
			mem_dc.SelectObject(&pen2);

			mem_dc.MoveTo(vertex_point[0].x, vertex_point[0].y);
			mem_dc.LineTo(vertex_point[2].x, vertex_point[2].y);

			mem_dc.MoveTo(vertex_point[1].x, vertex_point[1].y);
			mem_dc.LineTo(vertex_point[3].x, vertex_point[3].y);
		}
	}
	else if (shape == 0)
	{
		if (viewable == 0)
		{
			mem_dc.SelectObject(&pen2);
			ScalePattern(rect, vertex_point);
			mem_dc.Ellipse(vertex_point[0].x, vertex_point[0].y, vertex_point[1].x, vertex_point[1].y);
		}
		else if ((viewable == 1))
		{
			mem_dc.SelectObject(&pen1);
			ScalePattern(rect, vertex_point);
			mem_dc.Ellipse(vertex_point[0].x, vertex_point[0].y, vertex_point[1].x, vertex_point[1].y);
		}
	}
	pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);

}


void CPolaCustomUi::OnBnClickedButton1()
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

	Invalidate();
	UpdateWindow();
	// TODO: Add your control notification handler code here
}

void CPolaCustomUi::ScalePattern(CRect& rect, CPoint vertex_point[4])
{
	int tb = int(0.6 * rect.Width());
	int th = int(0.6 * rect.Height());

	double b_scale = b / tb;
	double h_scale = h / th;

	int center_x = rect.Width() / 2;
	int center_y = rect.Height() / 2;
	if (shape == 1)
	{
		if (b_scale > 1 || h_scale > 1)
		{
			int tscale = int(BasicTools::Max(b_scale, h_scale));

			vertex_point[0].SetPoint(center_x - int(0.5 * (b / tscale)), center_y + int(0.5 * (h / tscale)));
			vertex_point[1].SetPoint(center_x - int(0.5 * (b / tscale)), center_y - int(0.5 * (h / tscale)));
			vertex_point[2].SetPoint(center_x + int(0.5 * (b / tscale)), center_y - int(0.5 * (h / tscale)));
			vertex_point[3].SetPoint(center_x + int(0.5 * (b / tscale)), center_y + int(0.5 * (h / tscale)));
		}
		else
		{
			int tscale = (tb / b) > (th / h) ? int(tb / b) : int(th / h);

			vertex_point[0].SetPoint(center_x - int(0.5 * (b * tscale)), center_y + int(0.5 * (h * tscale)));
			vertex_point[1].SetPoint(center_x - int(0.5 * (b * tscale)), center_y - int(0.5 * (h * tscale)));
			vertex_point[2].SetPoint(center_x + int(0.5 * (b * tscale)), center_y - int(0.5 * (h * tscale)));
			vertex_point[3].SetPoint(center_x + int(0.5 * (b * tscale)), center_y + int(0.5 * (h * tscale)));
		}
	}
	else if (shape == 0)
	{

		if (b_scale > 1 || h_scale > 1)
		{
			int tscale = int(BasicTools::Max(b_scale, h_scale));

			vertex_point[0].SetPoint(center_x - int((b * 0.5 / tscale)), center_y - int((b * 0.5 / tscale)));
			vertex_point[1].SetPoint(center_x + int((b * 0.5 / tscale)), center_y + int((b * 0.5 / tscale)));
		}
		else
		{
			int tscale = (tb / b) > (th / h) ? int(tb / b) : int(th / h);

			vertex_point[0].SetPoint(center_x - int((b * 0.5 * tscale)), center_y - int((b * 0.5 * tscale)));
			vertex_point[1].SetPoint(center_x + int((b * 0.5 * tscale)), center_y + int((b * 0.5 * tscale)));
		}
	}
}

