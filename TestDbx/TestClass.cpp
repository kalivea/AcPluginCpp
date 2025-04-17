#include "StdAfx.h"
#include "TestClass.h"
#include "PolaReactor.h"
#include "PolaIRM.h"
#include "DwgTools.h"
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
	//DwgTools::CopyBlockDefFromOtherDwg(_T("C:\\Users\\Polaris\\Documents\\blk.dwg"), _T("blocktest"));
	DwgTools::InsertDwgBlockDef(_T("C:\\Users\\Polaris\\Documents\\outdwg.dwg"), _T("blocktest2"), true);
}
