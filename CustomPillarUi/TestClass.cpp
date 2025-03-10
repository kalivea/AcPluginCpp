#include "stdafx.h"
#include "TestClass.h"
#include "PolaPillarUi.h"
#include "StyleTools.h"
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestPillar"), _T("TestPillar"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	StyleTools::CreateLayerStyle(_T("POLA_PILLAR_STRUCTURE"), 2, _T("CONTINUOUS"));
	StyleTools::CreateLayerStyle(_T("POLA_PILLAR_MARK"), 3, _T("CONTINUOUS"));
	CAcModuleResourceOverride res;			// TODO
	CPolaPillarUi ui;
	if (ui.DoModal() != IDOK)
	{
		return;
	}
}