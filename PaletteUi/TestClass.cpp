#include "stdafx.h"
#include "TestClass.h"
#include "PolaPalette.h"
#include "PolaPaletteSet.h"

extern CPolaPaletteSet* palette_set;
extern CPolaPalette* pola_palette;
extern CAdUiPalette* cad_palette;
namespace
{
template<class T> bool destroyPalette(T*& palette)
{
    if (palette == nullptr) return true;
    // Detach while the set still exists. Its palette list must never retain a
    // pointer to a child C++ object that this module has deleted.
    if (palette_set != nullptr && palette_set->GetTabIndex(palette) >= 0 && !palette_set->RemovePalette(palette))
    {
        acutPrintf(_T("\nFailed to detach palette."));
        return false;
    }
    if (::IsWindow(palette->GetSafeHwnd()) && !palette->DestroyWindow())
    {
        acutPrintf(_T("\nFailed to destroy palette."));
        return false;
    }
    delete palette;
    palette = nullptr;
    return true;
}
bool destroyPaletteUi()
{
    if (!destroyPalette(pola_palette) || !destroyPalette(cad_palette)) return false;
    if (palette_set != nullptr)
    {
        if (::IsWindow(palette_set->GetSafeHwnd()) && !palette_set->DestroyWindow())
        {
            acutPrintf(_T("\nFailed to destroy palette set."));
            return false;
        }
        delete palette_set;
        palette_set = nullptr;
    }
    return true;
}
void creationFailed()
{
    acutPrintf(_T("\nFailed to create palette UI."));
    destroyPaletteUi(); // Any cleanup failure is reported and retains its owner.
}
}
void TestClass::TestClassInit()
{
    acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestPalette"), _T("TestPalette"), ACRX_CMD_MODAL, Test);
}
bool TestClass::TestClassUnload()
{
    if (!destroyPaletteUi()) return false;
    acedRegCmds->removeGroup(_T("tmpGroupName"));
    return true;
}
void TestClass::Test()
{
    CAcModuleResourceOverride resource_scope;
    CFrameWnd* frame = acedGetAcadFrame();
    if (frame == nullptr || !::IsWindow(frame->GetSafeHwnd())) return;
    // Failed creation can leave a retained object only when its cleanup failed;
    // retry cleanup before re-creating or using the partially built set.
    if (palette_set != nullptr && (!::IsWindow(palette_set->GetSafeHwnd()) || pola_palette == nullptr || cad_palette == nullptr || palette_set->GetPaletteCount() != 2))
    {
        if (!destroyPaletteUi()) return;
    }
    if (palette_set == nullptr)
    {
        palette_set = new CPolaPaletteSet();
        const CRect rect(0, 0, 400, 400);
        if (!palette_set->Create(_T("Pola Palette"), WS_OVERLAPPED | WS_DLGFRAME, rect, frame,
            PSS_EDIT_NAME | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP | PSS_CLOSE_BUTTON))
        {
            creationFailed();
            return;
        }
        pola_palette = new CPolaPalette();
        if (!pola_palette->Create(WS_CHILD | WS_VISIBLE, _T("palette 1"), palette_set, PS_EDIT_NAME) || palette_set->AddPalette(pola_palette) < 0)
        {
            creationFailed();
            return;
        }
        cad_palette = new CAdUiPalette();
        if (!cad_palette->Create(WS_CHILD | WS_VISIBLE, _T("palette 2"), palette_set, PS_EDIT_NAME) || palette_set->AddPalette(cad_palette) < 0)
        {
            creationFailed();
            return;
        }
        palette_set->EnableDocking(CBRS_ALIGN_ANY);
        palette_set->RestoreControlBar();
    }
    frame->ShowControlBar(palette_set, TRUE, FALSE);
    if (palette_set->GetOpacity() != 100) palette_set->SetOpacity(100);
}
