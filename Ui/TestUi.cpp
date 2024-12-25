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
	acutPrintf(_T("Ui"));
	CPolaCustomUi ui;
	ui.DoModal();
	if (ui.DoModal() == IDOK)
	{
		return;
	}
}
