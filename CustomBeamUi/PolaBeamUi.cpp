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
#include "EditEntity.h"
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolaBeamUi, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CPolaBeamUi, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_PICKPILLAR, &CPolaBeamUi::OnBnClickedButtonPickpillar)
	ON_BN_CLICKED(IDC_BUTTON_PICKOFFSET, &CPolaBeamUi::OnBnClickedButtonPickoffset)
	ON_BN_CLICKED(IDC_BUTTON_EDITVIEWABLE, &CPolaBeamUi::OnBnClickedButtonEditviewable)
	ON_BN_CLICKED(IDC_BUTTON_ADDVERTEX, &CPolaBeamUi::OnBnClickedButtonAddvertex)
	ON_BN_CLICKED(IDC_BUTTON_ADDJOINT, &CPolaBeamUi::OnBnClickedButtonAddjoint)
	ON_EN_KILLFOCUS(IDC_EDIT_BEAM_SN, &CPolaBeamUi::OnEnKillfocusEditBeamSn)
	ON_EN_KILLFOCUS(IDC_EDIT_BEAM_B, &CPolaBeamUi::OnEnKillfocusEditBeamB)
	ON_EN_KILLFOCUS(IDC_EDIT_BEAM_H, &CPolaBeamUi::OnEnKillfocusEditBeamH)
	ON_EN_KILLFOCUS(IDC_EDIT_BEAM_SLAB, &CPolaBeamUi::OnEnKillfocusEditBeamSlab)
	ON_EN_KILLFOCUS(IDC_EDIT_BEAM_SLABOFFSET, &CPolaBeamUi::OnEnKillfocusEditBeamSlaboffset)
	ON_BN_CLICKED(IDC_BUTTON_ADDINFO, &CPolaBeamUi::OnBnClickedButtonAddinfo)
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
	if (!InputValidator<int>::Validate(Edit_Beam_Sn_, beam_Sn, _T("Beam SN")))
	{
		Edit_Beam_Sn_.SetFocus();
		return;
	}

	if (!InputValidator<double>::Validate(Edit_Beam_b_, beam_b, _T("Beam width")))
	{
		Edit_Beam_b_.SetFocus();
		return;
	}

	if (!InputValidator<double>::Validate(Edit_Beam_h_, beam_h, _T("Beam height")))
	{
		Edit_Beam_h_.SetFocus();
		return;
	}

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
	AcDbObjectId beam_id = CPolaCustomBeam::SelectPillarDrawBeam(beam);
	CompleteEditorCommand();
	if (beam_id != AcDbObjectId::kNull)
		EditEntity::SetLayer(beam_id, _T("POLA_BEAM_STRUCTURE"));
}

void CPolaBeamUi::OnBnClickedButtonPickoffset()
{
	if (!InputValidator<int>::Validate(Edit_Beam_Sn_, beam_Sn, _T("Beam SN")))
	{
		Edit_Beam_Sn_.SetFocus();
		return;
	}

	if (!InputValidator<double>::Validate(Edit_Beam_b_, beam_b, _T("Beam width")))
	{
		Edit_Beam_b_.SetFocus();
		return;
	}

	if (!InputValidator<double>::Validate(Edit_Beam_h_, beam_h, _T("Beam height")))
	{
		Edit_Beam_h_.SetFocus();
		return;
	}
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
	AcDbObjectId beam_id = AcDbObjectId::kNull;
	switch (selected_radio_id)
	{
	case IDC_RADIO_TOP:
		BeginEditorCommand();
		beam_id = CPolaCustomBeam::PickTopPointDrawBeam(beam);
		EditEntity::SetLayer(beam_id, _T("POLA_BEAM_STRUCTURE"));
		break;
	case IDC_RADIO_CEN:
		BeginEditorCommand();
		beam_id = CPolaCustomBeam::PickCenterPointDrawBeam(beam);
		EditEntity::SetLayer(beam_id, _T("POLA_BEAM_STRUCTURE"));
		break;
	case IDC_RADIO_BOT:
		BeginEditorCommand();
		beam_id = CPolaCustomBeam::PickBottomPointDrawBeam(beam);
		EditEntity::SetLayer(beam_id, _T("POLA_BEAM_STRUCTURE"));
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

	AcGePoint3d point;
	AcDbObjectPointer<CPolaCustomBeam> beam;

	while (true)
	{
		if (!SelectEntitys::PickPoint(_T("Select point: \n"), point))
		{

			acutPrintf(_T("Point selection canceled. Returning to dialog.\n"));
			break;
		}
		beam.open(beam_id.at(0), OpenMode::kForWrite);
		CPolaCustomBeam::ModifyViewable(beam, beam->GetSegmentIndex(point) + 1);
	}
	CompleteEditorCommand();
}

void CPolaBeamUi::OnBnClickedButtonAddvertex()
{
	BeginEditorCommand();
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

	AcDbObjectPointer<CPolaCustomBeam> beam;
	AcGePoint3d point;
	if (SelectEntitys::PickPoint(_T("Select point: \n"), point))
	{
		CompleteEditorCommand();
		beam.open(beam_id.at(0), OpenMode::kForWrite);
		beam->InsertVertex(point);
	}
	else
	{
		CompleteEditorCommand();
	}
}

void CPolaBeamUi::OnBnClickedButtonAddjoint()
{
	if (!InputValidator<double>::Validate(Edit_Beam_Slab, slab_thickness, _T("Slab thickness")))
	{
		Edit_Beam_Slab.SetFocus();
		return;
	}
	if (!InputValidator<double>::Validate(Edit_Beam_Slab_offset, offset_length, _T("Slab offset length")))
	{
		Edit_Beam_Slab_offset.SetFocus();
		return;
	}
	BeginEditorCommand();
	AcDbObjectIdArray beam_id;
	bool isSelectionSuccessful = false;

	while (!isSelectionSuccessful)
	{
		if (SelectEntitys::PickEntitys(_T("Pick beam\n"), CPolaCustomBeam::desc(), beam_id))
		{
			if (beam_id.length() > 1)
			{
				acutPrintf(_T("\nOnly pick one beam entity. Please try again.\n"));
				beam_id.setLogicalLength(0);
				continue;
			}
			isSelectionSuccessful = true;
		}
		else
		{
			acutPrintf(_T("\nSelection canceled.\n"));
			CompleteEditorCommand();
			return;
		}
	}
	CompleteEditorCommand();
	AcDbObjectPointer<CPolaCustomBeam> beam;
	beam.open(beam_id.at(0), OpenMode::kForWrite);
	EditEntity::SetLayer(beam->addJoint(slab_thickness, offset_length), _T("POLA_BEAM_JOINT"));
}

void CPolaBeamUi::OnEnKillfocusEditBeamSn()
{
	InputValidator<int>::Validate(Edit_Beam_Sn_, beam_Sn, _T("Beam SN"), false);
}

void CPolaBeamUi::OnEnKillfocusEditBeamB()
{
	InputValidator<double>::Validate(Edit_Beam_b_, beam_b, _T("Beam width"), false);
}

void CPolaBeamUi::OnEnKillfocusEditBeamH()
{
	InputValidator<double>::Validate(Edit_Beam_h_, beam_h, _T("Beam height"), false);
}

void CPolaBeamUi::OnEnKillfocusEditBeamSlab()
{
	InputValidator<double>::Validate(Edit_Beam_Slab, slab_thickness, _T("slab thickness"), false);
}

void CPolaBeamUi::OnEnKillfocusEditBeamSlaboffset()
{
	InputValidator<double>::Validate(Edit_Beam_Slab_offset, offset_length, _T("slab thickness"), false);
}

void CPolaBeamUi::OnBnClickedButtonAddinfo()
{
	BeginEditorCommand();
	AcDbObjectIdArray beam_id;
	bool isSelectionSuccessful = false;

	while (!isSelectionSuccessful)
	{
		if (SelectEntitys::PickEntitys(_T("Pick beam\n"), CPolaCustomBeam::desc(), beam_id))
		{
			if (beam_id.length() > 1)
			{
				acutPrintf(_T("\nOnly pick one beam entity. Please try again.\n"));
				beam_id.setLogicalLength(0);
				continue;
			}
			isSelectionSuccessful = true;
		}
		else
		{
			acutPrintf(_T("\nSelection canceled.\n"));
			CompleteEditorCommand();
			return;
		}
	}
	CompleteEditorCommand();
	AcDbObjectPointer<CPolaCustomBeam> beam;
	beam.open(beam_id.at(0), OpenMode::kForWrite);
	EditEntity::SetLayer(beam->addBeamSnInfo(), _T("POLA_BEAM_MARK"));
}
