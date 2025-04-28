#include "stdafx.h"
#include "TestClass.h"

#include "resource.h"
#include "acadi_i.c"

extern bool isMenuLoad;
void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestMenu"), _T("TestMenu"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
	IAcadApplication* pAcad;
	IAcadMenuBar* pMenuBar;
	IAcadMenuGroups* pMenuGroups;
	IAcadMenuGroup* pMenuGroup;
	IAcadPopupMenus* pPopUpMenus;
	IAcadPopupMenu* pPopUpMenu;
	IAcadPopupMenuItem* pPopUpMenuItem;

	HRESULT hr = NOERROR;
	LPUNKNOWN pUnk = NULL;
	LPDISPATCH pAcadDisp = acedGetIDispatch(true);
	hr = pAcadDisp->QueryInterface(IID_IAcadApplication, (void**)&pAcad);
	pAcadDisp->Release();
	if (FAILED(hr))
		return;
	pAcad->put_Visible(true);
	pAcad->get_MenuBar(&pMenuBar);
	pAcad->get_MenuGroups(&pMenuGroups);
	pAcad->Release();

	long numberOfMenus;
	pMenuBar->get_Count(&numberOfMenus);
	pMenuBar->Release();

	VARIANT index;
	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = 0;
	pMenuGroups->Item(index, &pMenuGroup);
	pMenuGroups->Release();
	pMenuGroup->get_Menus(&pPopUpMenus);
	pMenuGroup->Release();

	WCHAR wstrMenuName[256];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "标准图库", -1, wstrMenuName, 256); 

	if (!isMenuLoad)
	{
		pPopUpMenus->Add(wstrMenuName, &pPopUpMenu);
		if (pPopUpMenu != NULL)
		{
			pPopUpMenu->put_Name(wstrMenuName);

			// Define the structure for menus and submenus
			struct SubMenuItem {
				const char* name;          // Submenu item name
				const char* macro;         // Macro 
			};

			struct MenuItem {
				const char* name;          // Top-level menu name
				SubMenuItem items[10];     // Submenu items
				int itemCount;             // Number of submenu items
			};

			MenuItem menuItems[] = {
				{
					"明挖(&M)",
					{
						{"灌注桩(&G)", "gzz "},
						{"地下连续墙(&D)", "dxlxq "},
						{"咬合桩(&Y)", "yhz "},
						{"SMW工法桩(&S)", "smwgfz "},
						{"TRD(&T)", "trd "},
						{"钢支撑及钢围檩(&W)", "gzc "},
						{"格构柱(&J)", "ggz "}
					},
					7										// numbers of sub menu
				},
				{
					"盖挖(&G)",
					{
						{"盖挖测试(&C)", "gwcs "}
					},
					1
				},
				{
					"暗挖(&A)",
					{
						{"暗挖测试(&C)", "awcs "}
					},
					1
				},
				{
					"盾构(&D)",
					{
						{"6.6m盾构(&6)", "6.6mdg "},
						{"8.5m盾构(&8)", "8.5mdg "}
					},
					2
				}
			};

			int menuItemCount = sizeof(menuItems) / sizeof(MenuItem);

			// Loop through each top-level menu
			for (int i = 0; i < menuItemCount; ++i)
			{
				// Convert top-level menu name to wide string
				WCHAR wstrTopMenuName[256];
				MultiByteToWideChar(CP_ACP, 0, menuItems[i].name, -1, wstrTopMenuName, 256);

				// Add sub-menu under the main menu
				IAcadPopupMenu* pSubMenu;
				VariantInit(&index);
				V_VT(&index) = VT_I4;
				V_I4(&index) = i;
				pPopUpMenu->AddSubMenu(index, wstrTopMenuName, &pSubMenu);

				if (pSubMenu != NULL)
				{
					// Loop through each submenu item
					for (int j = 0; j < menuItems[i].itemCount; ++j)
					{
						WCHAR wstrSubMenuItemName[256];
						WCHAR wstrSubMenuItemMacro[256];

						// Convert submenu item name and macro to wide string
						MultiByteToWideChar(CP_ACP, 0, menuItems[i].items[j].name, -1, wstrSubMenuItemName, 256);
						MultiByteToWideChar(CP_ACP, 0, menuItems[i].items[j].macro, -1, wstrSubMenuItemMacro, 256);

						// Add submenu item
						VariantInit(&index);
						V_VT(&index) = VT_I4;
						V_I4(&index) = j;
						pSubMenu->AddMenuItem(index, wstrSubMenuItemName, wstrSubMenuItemMacro, &pPopUpMenuItem);
						pPopUpMenuItem->Release();
					}
					pSubMenu->Release();
				}
			}

			// Insert the main menu into the menu bar
			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = numberOfMenus - 2;  // Adjust position as needed
			pPopUpMenu->InsertInMenuBar(index);

			pPopUpMenu->Release();
			isMenuLoad = true;
		}
		else
		{
			acutPrintf(_T("Failed to create the menu.\n"));
		}
	}
	pPopUpMenus->Release();
}
