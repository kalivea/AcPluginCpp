#include "stdafx.h"
#include "TestClass.h"
#include "PolaMenuDlgBar.h"
#include "PolaMenu.h"

extern CPolaMenuDlgBar* pbar;
extern PolaMenu* pMenu;
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
	AfxSetResourceHandle(_hdllInstance);
	pbar = new CPolaMenuDlgBar;
	pbar->Create(acedGetAcadFrame(), _T("text"));
	AfxSetResourceHandle(acedGetAcadResourceInstance()); pbar->EnableDocking(CBRS_ALIGN_ANY);
	acedGetAcadFrame()->DockControlBar(pbar, CBRS_ALIGN_TOP);
	pbar->SetWindowText(_T("pbar"));
	acedGetAcadFrame()->ShowControlBar(pbar, TRUE, FALSE);
	acedGetAcadFrame()->RecalcLayout();//重新排列窗口 //AcUiMainWindow()->RedrawWindow();
}