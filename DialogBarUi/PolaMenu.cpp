#include "stdafx.h"
#include "PolaMenu.h"

PolaMenu::PolaMenu()
{
}

PolaMenu::~PolaMenu()
{
}

void PolaMenu::Set(CStringArray& menu, int& id)
{
	bool bCreateNew = false;
	menu_.CreateMenu();
	while (menu.GetSize() > 0)
	{
		CString csStr = menu[0];
		menu.RemoveAt(0);
		csStr.TrimLeft();
		if (_T("") == csStr)
			continue;
		csStr.MakeUpper();
		if (_T("***POP") == csStr.Left(6))
		{
			bCreateNew = true;
			continue;
		}
		if (bCreateNew)
		{
			bCreateNew = false;
			csStr.Replace(_T("["), _T(""));
			csStr.Replace(_T("]"), _T(""));
			CMenu subMenu;
			if (CreatePOP(menu, id, subMenu))
				menu_.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, csStr);
		}
	}
}

bool PolaMenu::CreatePOP(CStringArray& menu, int& id, CMenu& sub_menu)
{
	return false;
}

bool PolaMenu::CreateSUB(CStringArray& menu, int& id, CMenu& sub_menu)
{
	return false;
}
