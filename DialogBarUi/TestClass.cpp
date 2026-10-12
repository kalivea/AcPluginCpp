#include "stdafx.h"
#include "TestClass.h"
#include "PolaDialogBar.h"
#include <memory>

namespace { CPolaDialogBar* dialog_bar = nullptr; }
void TestClass::TestClassInit()
{
    acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestBar"), _T("TestBar"), ACRX_CMD_MODAL, Test);
}
bool TestClass::TestClassUnload()
{
    if (dialog_bar != nullptr)
    {
        if (::IsWindow(dialog_bar->GetSafeHwnd()) && !dialog_bar->DestroyWindow())
        {
            acutPrintf(_T("\nFailed to destroy dialog bar."));
            return false;
        }
        delete dialog_bar;
        dialog_bar = nullptr;
    }
    acedRegCmds->removeGroup(_T("tmpGroupName"));
    return true;
}
void TestClass::Test()
{
    CAcModuleResourceOverride resource_scope;
    CFrameWnd* frame = acedGetAcadFrame();
    if (frame == nullptr || !::IsWindow(frame->GetSafeHwnd())) return;
    if (dialog_bar != nullptr && !::IsWindow(dialog_bar->GetSafeHwnd()))
    {
        delete dialog_bar;
        dialog_bar = nullptr;
    }
    if (dialog_bar == nullptr)
    {
        std::unique_ptr<CPolaDialogBar> candidate(new CPolaDialogBar);
        if (!candidate->Create(frame, _T("bar")))
        {
            acutPrintf(_T("\nFailed to create dialog bar."));
            return;
        }
        candidate->EnableDocking(CBRS_ALIGN_ANY);
        frame->DockControlBar(candidate.get(), CBRS_ALIGN_TOP);
        candidate->SetWindowText(_T("pbar"));
        dialog_bar = candidate.release();
    }
    dialog_bar->m_bIsShow = TRUE;
    frame->ShowControlBar(dialog_bar, TRUE, FALSE);
    frame->RecalcLayout();
    if (AcUiMainWindow() != nullptr) AcUiMainWindow()->RedrawWindow();
    AdUiShowDockControlBars(true);
}
