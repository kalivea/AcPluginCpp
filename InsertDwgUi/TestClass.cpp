#include "stdafx.h"
#include "TestClass.h"
#include "OptionSheet.h"
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestInsert"), _T("TestInsert"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	CAcModuleResourceOverride res;			// TODO
	COptionSheet ui;
	if (ui.DoModal() != IDOK)
	{
		return;
	}
}