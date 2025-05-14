#include "stdafx.h"
#include "TestClass.h"


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
	IAcadApplication* pAcad = nullptr;
	IAcadMenuBar* pMenuBar = nullptr;
	IAcadMenuGroups* pMenuGroups = nullptr;
	IAcadMenuGroup* pMenuGroup = nullptr;
	IAcadPopupMenus* pPopUpMenus = nullptr;
	IAcadPopupMenu* pPopUpMenu = nullptr;

	HRESULT hr = NOERROR;
	LPDISPATCH pAcadDisp = acedGetIDispatch(TRUE);
	hr = pAcadDisp->QueryInterface(IID_IAcadApplication, (void**)&pAcad);
	pAcadDisp->Release();
	if (FAILED(hr)) return;

	pAcad->put_Visible(VARIANT_TRUE);
	pAcad->get_MenuBar(&pMenuBar);
	pAcad->get_MenuGroups(&pMenuGroups);
	pAcad->Release();

	long numberOfMenus = 0;
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
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "��׼ͼ��", -1, wstrMenuName, 256);

	if (!isMenuLoad)
	{
		pPopUpMenus->Add(wstrMenuName, &pPopUpMenu);
		if (pPopUpMenu != NULL)
		{
			pPopUpMenu->put_Name(wstrMenuName);

			const MenuItem menuItems[] = {
				{
					"���½ṹרҵ(&U)",
					nullptr,
					[] {
						static const MenuItem sub[] = {
							{
								"�������ڵ����ͼ(&M)",
								nullptr,
								[] {
									static const MenuItem subsub[] = {
										{
											"����(&M)",
											nullptr,
											[] {
												static const MenuItem mingwaSub[] = {
													{"��ע׮(&G)", "foundation_pile ", nullptr, 0},
													{"����ǽ(&D)", "TestDLQ ", nullptr, 0},
													{"ҧ��׮(&Y)", "TestYHZ ", nullptr, 0},
													{"SMW����׮(&S)", "secant_pile ", nullptr, 0},
													{"����(&F)", "secant_pile ", nullptr, 0},
													{"����ǽ(&T)", "secant_pile ", nullptr, 0},
													{"TRD(&T)", "secant_pile ", nullptr, 0},
													{"��֧�ż���Χ��(&T)", "TestGZC ", nullptr, 0},
													{"����(&G)", "secant_pile ", nullptr, 0},
													{"�˹��ڿ�׮(&Y)", "secant_pile ", nullptr, 0}
												};
												return mingwaSub;
											}(),
											10
										},
										{"����(&C)", "underground_gaiwa ", nullptr, 0},
										{"����(&A)", "underground_anwa ", nullptr, 0},
										{"�ܹ�(&S)", "underground_dungou ", nullptr, 0}
									};
									return subsub;
								}(),
								4
							},
							{
								"��������׼��ͼֽ(&R)",
								nullptr,
								[] {
									static const MenuItem subsub[] = {
										{"���(&T)", "standard_tianjin ", nullptr, 0},
										{"����(&S)", "standard_shenzhen ", nullptr, 0},
										{"��ɳ(&C)", "standard_changsha ", nullptr, 0}
									};
									return subsub;
								}(),
								3
							}
						};
						return sub;
					}(),
					2
				},
				{
					"�����ṹרҵ(&B)",
					"building_structure ",
					nullptr,
					0
				}
			};
			AddMenuItems(pPopUpMenu, menuItems, sizeof(menuItems) / sizeof(MenuItem));

			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = numberOfMenus - 2;
			pPopUpMenu->InsertInMenuBar(index);

			pPopUpMenu->Release();
			isMenuLoad = true;
		}
		else
		{
			acutPrintf(_T("�����˵�ʧ��!\n"));
		}
	}
	pPopUpMenus->Release();
}

void TestClass::AddMenuItems(IAcadPopupMenu* pParentMenu, const MenuItem* items, int itemCount)
{
	for (int i = 0; i < itemCount; ++i)
	{
		const MenuItem& item = items[i];
		VARIANT idx;
		VariantInit(&idx);
		V_VT(&idx) = VT_I4;
		V_I4(&idx) = i;

		WCHAR wName[256];
		MultiByteToWideChar(CP_ACP, 0, item.name, -1, wName, 256);

		if (item.subItems && item.subItemCount > 0)
		{
			IAcadPopupMenu* pSubMenu = nullptr;
			if (SUCCEEDED(pParentMenu->AddSubMenu(idx, wName, &pSubMenu)) && pSubMenu)
			{
				AddMenuItems(pSubMenu, item.subItems, item.subItemCount);
				pSubMenu->Release();
			}
		}
		else
		{
			WCHAR wMacro[256] = { 0 };
			if (item.macro)
				MultiByteToWideChar(CP_ACP, 0, item.macro, -1, wMacro, 256);

			IAcadPopupMenuItem* pMenuItem = nullptr;
			if (SUCCEEDED(pParentMenu->AddMenuItem(idx, wName, wMacro, &pMenuItem)) && pMenuItem)
				pMenuItem->Release();
		}
	}
}
