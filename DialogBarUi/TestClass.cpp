#include "stdafx.h"
#include "TestClass.h"
#include "PolaDialogBar.h"

void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestBar"), _T("TestBar"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	CAcModuleResourceOverride res;			// TODO
	CPolaDialogBar* pbar = nullptr;
	if (!pbar)
	{
		AfxSetResourceHandle(_hdllInstance);
		pbar = new CPolaDialogBar;
		if (!pbar->Create(acedGetAcadFrame(), _T("bar")))
		{
			TRACE0("Failed to create DlgBar\n");
			return; // fail to create
		}
		AfxSetResourceHandle(acedGetAcadResourceInstance());
		pbar->EnableDocking(CBRS_ALIGN_ANY);
		acedGetAcadFrame()->DockControlBar(pbar, CBRS_ALIGN_TOP);
		pbar->SetWindowText(_T("pbar"));
	}
	acedGetAcadFrame()->ShowControlBar(pbar, TRUE, FALSE);
	acedGetAcadFrame()->RecalcLayout();
	AcUiMainWindow()->RedrawWindow();
	AdUiShowDockControlBars(true);
}