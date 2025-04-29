#include "stdafx.h"  
#include "TestClass.h"  
#include "AcExtensionModule.h"  
#include <AdMenuBar.h>  
#include <AdApplicationFrame.h> 
#include "resource.h"

void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestMenuBar"), _T("TestMenuBar"), ACRX_CMD_MODAL, Test);
}

void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}

void TestClass::Test()
{
    Acad::ErrorStatus error = Acad::eOk;
    AdApplicationFrame* pNewAppFrame = acedGetApplicationFrame();
    pNewAppFrame != NULL ? error = Acad::eOk : error = Acad::eNullPtr;
    HMENU hDefaultMenu = NULL;  /* Needed to restore to Default ACAD Menu */
    HMENU hNewMenu = NULL;
    CString strDirName;
    HMODULE hm = NULL;

    CAcModuleResourceOverride hResources;

    if (error == Acad::eOk)
    {
        AdMenuBar* pMenuBar = NULL;
        pMenuBar = pNewAppFrame->GetMenuBar();
        pMenuBar != NULL ? error = Acad::eOk : error = Acad::eNullPtr;

        if (error == Acad::eOk)
        {
            hDefaultMenu = pMenuBar->GetMenuHandle();
            if (hDefaultMenu != 0)
            {
                // Load our custom menu file
                strDirName = _T("C:\\Users\\Polaris\\source\\repos\\AcPluginCpp\\x64\\Debug\\PolaMenuBarUi.arx");
                hm = GetModuleHandle(strDirName);
                if (hm != NULL)
                {
                    // Set custom menu
                    hNewMenu = LoadMenu(hm, MAKEINTRESOURCE(IDR_MENU1));
                    if (hNewMenu != NULL)
                    {
                        // Append the new menu at the end of the existing menu
                        AppendMenu(hDefaultMenu, MF_POPUP, (UINT_PTR)hNewMenu, _T("Custom Menu"));

                        // Set the updated menu handle
                        bool bOk = pMenuBar->SetMenuHandle(hDefaultMenu);
                        if (bOk)
                        {
                            pMenuBar->UpdateMenu();
                        }
                        else
                        {
                            error = (Acad::ErrorStatus)Acad::eFailed;
                        }
                    }
                    else
                    {
                        error = Acad::eNullHandle;
                    }
                }
                else
                {
                    error = Acad::eNullHandle;
                }
            }
            else
            {
                error = Acad::eNullHandle;
            }
        }
        else
        {
            error = Acad::eUnrecoverableErrors;
        }

        /* Make sure the MenuBar is visible */
        if (!pNewAppFrame->IsMenuBarVisible())
            pNewAppFrame->EnableMenuBar(true);
    }
    else
    {
        error = Acad::eNullPtr;
    }

    acutPrintf(_T("\n%s"), acadErrorStatusText(error));
}
