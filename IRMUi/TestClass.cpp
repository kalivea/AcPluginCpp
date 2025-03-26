#include "stdafx.h"
#include "TestClass.h"
#include "PolaIRMUi.h"
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestIRM"), _T("TestIRM"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	StyleTools::CreateTextStyle(_T("IRM_default"), _T("hztxt.shx"), _T("tssdeng.shx"));
	StyleTools::CreateLayerStyle(_T("POLA_IRM_MARK"), 3, _T("CONTINUOUS"));
	CAcModuleResourceOverride res;			// TODO
	CPolaIRMUi ui;
	if (ui.DoModal() != IDOK)
	{
		return;
	}
}