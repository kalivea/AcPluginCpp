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
IMPLEMENT_DYNAMIC(CPolaPillarUi, CAdUiBaseDialog)

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
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CPolaPillarUi::OnBnClickedButtonPreview)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaPillarUi::CPolaPillarUi(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog(CPolaPillarUi::IDD, pParent, hInstance) {
}

void CPolaPillarUi::ScalePattern(ShapeType shape, double d1, double d2, LineStyle style)
{
	if (d1 <= 0 || (shape == RECTANGLE && d2 <= 0))
		return;

	CWnd* picture = GetDlgItem(IDC_STATIC_PICTURE);
	if (!picture || !::IsWindow(picture->m_hWnd))
		return;

	CRect rc_picture;
	picture->GetClientRect(rc_picture);

	const double scale_factor = 0.75;
	CRect rc_target(0, 0, static_cast<int>(rc_picture.Width() * scale_factor), static_cast<int>(rc_picture.Height() * scale_factor));

	if (shape == RECTANGLE)
	{
		double aspect = d1 / d2;
		double target_aspect_ratio = static_cast<double>(rc_target.Width()) / rc_target.Height();

		if (aspect > target_aspect_ratio)
		{
			rc_target.bottom = rc_target.top + static_cast<int>(rc_target.Width() / aspect);
		}
		else
		{
			rc_target.right = rc_target.left + static_cast<int>(rc_target.Height() * aspect);
		}
	}
	else
	{
		int min_side = min(rc_target.Width(), rc_target.Height());
		rc_target.right = rc_target.left + min_side;
		rc_target.bottom = rc_target.top + min_side;
	}
	rc_target.OffsetRect((rc_picture.Width() - rc_target.Width()) / 2, (rc_picture.Height() - rc_target.Height()) / 2);
	rc_draw_rect = rc_target;
	shape_type = shape;
	line_style = style;
	isDataReady = TRUE;
	Invalidate(FALSE);
	UpdateWindow();
}

//-----------------------------------------------------------------------------
void CPolaPillarUi::DoDataExchange(CDataExchange* pDX) {
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
LRESULT CPolaPillarUi::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

void CPolaPillarUi::OnBnClickedRadioRect()
{
	int selected_id = GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC);
	bool enable_edit = (selected_id == IDC_RADIO_RECT);
	GetDlgItem(IDC_EDIT_D)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_H)->EnableWindow(enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_D)->EnableWindow(!enable_edit);
	GetDlgItem(IDC_EDIT_PIPE_T)->EnableWindow(!enable_edit);

	SetDlgItemText(IDC_EDIT_PIPE_D, _T(""));
	pipe_d = 0;
	SetDlgItemText(IDC_EDIT_PIPE_T, _T(""));
	pipe_t = 0;
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

	SetDlgItemText(IDC_EDIT_D, _T(""));
	pillar_d = 0;
	SetDlgItemText(IDC_EDIT_H, _T(""));
	pillar_h = 0;
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
	Edit_Pipe_D_.GetWindowTextW(temp);
	pipe_d = _wtof(temp);
	pillar_d = pipe_d;
}

void CPolaPillarUi::OnEnKillfocusEditPipeT()
{
	CString temp;
	Edit_Pipe_T_.GetWindowTextW(temp);
	pipe_t = _wtof(temp);

}

void CPolaPillarUi::OnEnKillfocusEditD()
{
	CString temp;
	Edit_Pillar_D_.GetWindowTextW(temp);
	pillar_d = _wtof(temp);
}

void CPolaPillarUi::OnEnKillfocusEditH()
{
	CString temp;
	Edit_Pillar_H_.GetWindowTextW(temp);
	pillar_h = _wtof(temp);
}

void CPolaPillarUi::OnBnClickedButtonPreview()
{
	if (GetCheckedRadioButton(IDC_RADIO_RECT, IDC_RADIO_CIRC) == IDC_RADIO_RECT)
	{
		if (GetCheckedRadioButton(IDC_RADIO_SLINE, IDC_RADIO_DLINE) == IDC_RADIO_SLINE)
		{
			ScalePattern(RECTANGLE, pillar_d, pillar_h, SOLID);
		}
		else
		{
			ScalePattern(RECTANGLE, pillar_d, pillar_h, DASHED);
		}
	}
	else
	{
		if (GetCheckedRadioButton(IDC_RADIO_SLINE, IDC_RADIO_DLINE) == IDC_RADIO_SLINE)
		{
			ScalePattern(CIRCLE, pipe_d, 0, SOLID);
		}
		else
		{
			ScalePattern(CIRCLE, pipe_d, 0, DASHED);
		}
	}
}

void CPolaPillarUi::OnPaint()
{
	CPaintDC dc(this);
	CWnd* picture = GetDlgItem(IDC_STATIC_PICTURE);
	if (!picture) return;

	CRect rc_picture;
	picture->GetWindowRect(&rc_picture);
	ScreenToClient(&rc_picture);

	CDC mem_dc;
	mem_dc.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rc_picture.Width(), rc_picture.Height());
	CBitmap* old_bitmap = mem_dc.SelectObject(&bitmap);

	mem_dc.FillSolidRect(0, 0, rc_picture.Width(), rc_picture.Height(), ::GetSysColor(COLOR_3DFACE));

	CPen pen_border, * old_pen;
	if (line_style == DASHED)
		pen_border.CreatePen(PS_DASH, 1, RGB(0, 0, 0));
	else
		pen_border.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	old_pen = mem_dc.SelectObject(&pen_border);
	mem_dc.SelectStockObject(NULL_BRUSH);

	if (shape_type == RECTANGLE)
	{
		mem_dc.Rectangle(rc_draw_rect);
		CPen diagonal_pen(PS_DASH, 1, RGB(0, 0, 0));
		CPen* old_diagonal_pen = mem_dc.SelectObject(&diagonal_pen);

		mem_dc.MoveTo(rc_draw_rect.TopLeft());
		mem_dc.LineTo(rc_draw_rect.BottomRight());
		mem_dc.MoveTo(rc_draw_rect.left, rc_draw_rect.bottom);
		mem_dc.LineTo(rc_draw_rect.right, rc_draw_rect.top);

		mem_dc.SelectObject(old_diagonal_pen);
		diagonal_pen.DeleteObject();

		if (isDataReady)
		{
			CString dimension;
			dimension.Format(_T("b¡Áh = %d ¡Á %d"), static_cast<int>(pillar_d), static_cast<int>(pillar_h));
			mem_dc.SetBkMode(TRANSPARENT);
			mem_dc.SetTextColor(RGB(0, 0, 0));

			CFont font;
			font.CreatePointFont(80, _T("Arial"));
			CFont* old_font = mem_dc.SelectObject(&font);

			CSize textSize = mem_dc.GetTextExtent(dimension);
			CRect textRect(0, rc_picture.Height() - textSize.cy - 5, rc_picture.Width(), rc_picture.Height());
			mem_dc.DrawText(dimension, &textRect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

			mem_dc.SelectObject(old_font);
			font.DeleteObject();
		}
	}
	else
	{
		mem_dc.Ellipse(rc_draw_rect);

		if (isDataReady) 
		{
			CString dimension;
			dimension.Format(_T("D = %d"), static_cast<int>(pipe_d));
			mem_dc.SetBkMode(TRANSPARENT);
			mem_dc.SetTextColor(RGB(0, 0, 0));

			CFont font;
			font.CreatePointFont(80, _T("Arial"));
			CFont* old_font = mem_dc.SelectObject(&font);

			CSize textSize = mem_dc.GetTextExtent(dimension);
			int x = (rc_picture.Width() - textSize.cx) / 2;
			int y = rc_picture.Height() - textSize.cy - 5;
			mem_dc.TextOut(x, y, dimension);

			mem_dc.SelectObject(old_font);
			font.DeleteObject();
		}
	}
	mem_dc.SelectObject(old_pen);
	pen_border.DeleteObject();
	dc.BitBlt(rc_picture.left, rc_picture.top, rc_picture.Width(), rc_picture.Height(), &mem_dc, 0, 0, SRCCOPY);
	mem_dc.SelectObject(old_bitmap);
	bitmap.DeleteObject();
}
