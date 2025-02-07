#include "StdAfx.h"
#include "TestUi.h"

void TestUi::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestUi"), _T("TestUi"), ACRX_CMD_MODAL, Test);
}

void TestUi::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}

void TestUi::Test()
{
	CPolaCustomUi ui;

	if (ui.DoModal() != IDOK)
	{
		return;
	}

	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	AcGePoint3dArray insert_point;
	//-----------------------------------------------------------------
	CPolaCustomPillar* pillar = new CPolaCustomPillar();
	pillar->setDiameter(ui.b, ui.h);
	pillar->setPillarType(ui.shape);
	pillar->setViewable(ui.viewable);
	pillar->setDirectionVector();
	pillar->setPillarProperty(0);
	pillar->setSn(1);
	pillar->setCenterPoint(AcGePoint3d(ui.x, ui.y, 0));
	pillar->CalculateVertex();
	pillar->AddPillarLeader();
	AddToModelSpace::AddEntityToModelSpace(pillar);
}
