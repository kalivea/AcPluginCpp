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
//----- PolaBeamUi.cpp : Implementation of CPolaBeamUi
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "PolaBeamUi.h"
#include <SelectEntitys.h>

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaBeamUi, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaBeamUi, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_PICKPILLAR, &CPolaBeamUi::OnBnClickedButtonPickpillar)
	ON_BN_CLICKED(IDC_BUTTON_PICKOFFSET, &CPolaBeamUi::OnBnClickedButtonPickoffset)
	ON_BN_CLICKED(IDC_BUTTON_EDITVIEWABLE, &CPolaBeamUi::OnBnClickedButtonEditviewable)
	ON_BN_CLICKED(IDC_BUTTON_ADDVERTEX, &CPolaBeamUi::OnBnClickedButtonAddvertex)
	ON_BN_CLICKED(IDC_BUTTON_ADDJOINT, &CPolaBeamUi::OnBnClickedButtonAddjoint)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CPolaBeamUi::CPolaBeamUi(CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog(CPolaBeamUi::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CPolaBeamUi::DoDataExchange(CDataExchange* pDX) {
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BEAM_SN, Edit_Beam_Sn_);
	DDX_Control(pDX, IDC_EDIT_BEAM_B, Edit_Beam_b_);
	DDX_Control(pDX, IDC_EDIT_BEAM_H, Edit_Beam_h_);
	DDX_Control(pDX, IDC_EDIT_BEAM_SLAB, Edit_Beam_Slab);
	DDX_Control(pDX, IDC_EDIT_BEAM_SLABOFFSET, Edit_Beam_Slab_offset);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CPolaBeamUi::OnAcadKeepFocus(WPARAM, LPARAM) {
	return (TRUE);
}

void CPolaBeamUi::OnBnClickedButtonPickpillar()
{
	CString temp;
	Edit_Beam_Sn_.GetWindowTextW(temp);
	beam_Sn = _wtoi(temp);

	Edit_Beam_b_.GetWindowTextW(temp);
	beam_b = _wtof(temp);

	Edit_Beam_h_.GetWindowTextW(temp);
	beam_h = _wtof(temp);

	AcDbObjectPointer<CPolaCustomBeam> beam;
	beam.create();
	if (beam.openStatus() != Acad::eOk)
	{
		return;
	}
	beam->setBeamProperty(beam_Sn);
	beam->setBeamWidth(beam_b);
	beam->setBeamHeight(beam_h);
	BeginEditorCommand();
	CPolaCustomBeam::SelectPillarDrawBeam(beam);
	CompleteEditorCommand();
}

void CPolaBeamUi::OnBnClickedButtonPickoffset()
{
	CString temp;
	Edit_Beam_Sn_.GetWindowTextW(temp);
	beam_Sn = _wtoi(temp);

	Edit_Beam_b_.GetWindowTextW(temp);
	beam_b = _wtof(temp);

	Edit_Beam_h_.GetWindowTextW(temp);
	beam_h = _wtof(temp);

	AcDbObjectPointer<CPolaCustomBeam> beam;
	beam.create();
	if (beam.openStatus() != Acad::eOk)
	{
		return;
	}
	int selected_radio_id = -1;
	selected_radio_id = GetCheckedRadioButton(IDC_RADIO_TOP, IDC_RADIO_BOT);

	beam->setBeamProperty(beam_Sn);
	beam->setBeamWidth(beam_b);
	beam->setBeamHeight(beam_h);

	switch (selected_radio_id)
	{
	case IDC_RADIO_TOP:
		BeginEditorCommand();
		CPolaCustomBeam::PickTopPointDrawBeam(beam);
		break;
	case IDC_RADIO_CEN:
		BeginEditorCommand();
		CPolaCustomBeam::PickCenterPointDrawBeam(beam);
		break;
	case IDC_RADIO_BOT:
		BeginEditorCommand();
		CPolaCustomBeam::PickBottomPointDrawBeam(beam);
		break;
	default:
		MessageBox(_T("No mode select!"));
		break;
	}
	CompleteEditorCommand();
}

void CPolaBeamUi::OnBnClickedButtonEditviewable()
{
	BeginEditorCommand();
	AcDbObjectIdArray beam_id;
	SelectEntitys::PickEntitys(_T("Pick beam\n"), CPolaCustomBeam::desc(), beam_id);

	if (beam_id.length() > 1)
		acutPrintf(_T("Only pick one beam entity"));

	AcGePoint3d point;
	AcDbObjectPointer<CPolaCustomBeam> beam;
	beam.create();
	while (SelectEntitys::PickPoint(_T("Select point: \n"), point))
	{
		beam.open(beam_id.at(0), OpenMode::kForWrite);
		CPolaCustomBeam::ModifyViewable(beam, beam->GetSegmentIndexByYProjection(point) + 1);
	}
	CompleteEditorCommand();
}

void CPolaBeamUi::OnBnClickedButtonAddvertex()
{
	BeginEditorCommand();
	AcDbObjectIdArray beam_id;
	if (!SelectEntitys::PickEntitys(_T("Pick beam\n"), CPolaCustomBeam::desc(), beam_id))
	{
		return;
	}
	if (beam_id.length() > 1)
	{
		acutPrintf(_T("Only pick one beam entity"));
		return;
	}

	AcDbObjectPointer<CPolaCustomBeam> beam;
	AcGePoint3d point;
	beam.open(beam_id.at(0), OpenMode::kForWrite);
	if (SelectEntitys::PickPoint(_T("Select point: \n"), point))
	{
		CompleteEditorCommand();
		beam.open(beam_id.at(0), OpenMode::kForWrite);
		beam->InsertVertex(point);
	}
	else
	{
		CancelEditorCommand();
	}
}

void CPolaBeamUi::OnBnClickedButtonAddjoint()
{
	CString temp;
	Edit_Beam_Slab.GetWindowTextW(temp);
	slab_thickness = _wtof(temp);
	Edit_Beam_Slab_offset.GetWindowTextW(temp);
	offset_length = _wtof(temp);

	BeginEditorCommand();
	AcDbObjectIdArray beam_id;
	if (!SelectEntitys::PickEntitys(_T("Pick beam\n"), CPolaCustomBeam::desc(), beam_id))
	{
		return;
	}
	if (beam_id.length() > 1)
	{
		acutPrintf(_T("Only pick one beam entity"));
		return;
	}
	CompleteEditorCommand();
	AcDbObjectPointer<CPolaCustomBeam> beam;
	beam.open(beam_id.at(0), OpenMode::kForWrite);
	beam->addJoint(slab_thickness, offset_length);
}
