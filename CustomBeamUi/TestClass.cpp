#include "stdafx.h"
#include "TestClass.h"
#include "PolaBeamUi.h"
#include "StyleTools.h"
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestClass"), _T("TestClass"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	CAcModuleResourceOverride res;			// TODO
	CPolaBeamUi ui;
	if (ui.DoModal() != IDOK)
	{
		return;
	}
}