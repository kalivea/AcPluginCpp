#include "stdafx.h"
#include "TestClass.h"
#include "PolaBeamUi.h"
#include "StyleTools.h"
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestBeam"), _T("TestBeam"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	StyleTools::CreateLayerStyle(_T("POLA_BEAM_STRUCTURE"), 7, _T("CONTINUOUS"));
	StyleTools::CreateLayerStyle(_T("POLA_BEAM_JOINT"), 7, _T("CONTINUOUS"));
	StyleTools::CreateLayerStyle(_T("POLA_BEAM_MARK"), 3, _T("CONTINUOUS"));
	CAcModuleResourceOverride res;			// TODO
	CPolaBeamUi ui;
	if (ui.DoModal() != IDOK)
	{
		return;
	}
}