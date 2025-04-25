#include "stdafx.h"
#include "TestClass.h"
#include "PolaPalette.h"
#include "PolaPaletteSet.h"

extern CPolaPaletteSet* palette_set;
extern CPolaPalette* pola_palette;
extern CAdUiPalette* cad_palette;
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestPalette"), _T("TestPalette"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));

	if (palette_set != nullptr)
	{
		palette_set->DestroyWindow();
		delete palette_set;
		palette_set = nullptr;
	}
	if (pola_palette != nullptr)
	{
		pola_palette->DestroyWindow();
		delete pola_palette;
		pola_palette = nullptr;
	}
	if (cad_palette != nullptr)
	{
		cad_palette->DestroyWindow();
		delete cad_palette;
		cad_palette = nullptr;
	}
}
void TestClass::Test()
{
	if (!palette_set)
	{
		palette_set = new CPolaPaletteSet();
		CRect rect(0, 0, 400, 400);
		palette_set->Create(_T("Pola Palette"), 
			WS_OVERLAPPED | WS_DLGFRAME, 
			rect, 
			acedGetAcadFrame(), 
			PSS_EDIT_NAME | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP | PSS_CLOSE_BUTTON);
		pola_palette = new CPolaPalette();
		cad_palette = new CAdUiPalette();

		pola_palette->Create(WS_CHILD | WS_VISIBLE, _T("palette 1"), palette_set, PS_EDIT_NAME);
		palette_set->AddPalette(pola_palette);

		cad_palette->Create(WS_CHILD | WS_VISIBLE, _T("palette 2"), palette_set, PS_EDIT_NAME);
		palette_set->AddPalette(cad_palette);

		palette_set->EnableDocking(CBRS_ALIGN_ANY);
		palette_set->RestoreControlBar();
	}
	acedGetAcadFrame()->ShowControlBar(palette_set, true, false);
	if (palette_set->GetOpacity() != 100)
		palette_set->SetOpacity(100);
}