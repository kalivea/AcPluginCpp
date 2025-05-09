#include "stdafx.h"
#include "CPolaMenu.h"

CPolaMenu::CPolaMenu()
{
}

CPolaMenu::~CPolaMenu()
{
}

void CPolaMenu::Set(CStringArray& saMenu, int& nID)
{
	bool isNewMenu = false;
	m_menu.CreateMenu();
	while (saMenu.GetSize() > 0)
	{
		CString csStr = saMenu[0];
		saMenu.RemoveAt(0);
		csStr.TrimLeft();
		if (_T("") == csStr)
			continue;
		csStr.MakeUpper();
		if (_T("***POP") == csStr.Left(6))
		{
			isNewMenu = true;
			continue;
		}
		if (isNewMenu)
		{
			isNewMenu = false;
			csStr.Replace(_T("["), _T(""));
			csStr.Replace(_T("]"), _T(""));
			CMenu subMenu;
			if (CreatePOP(saMenu, nID, subMenu))
				m_menu.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, csStr);
		}
	}
}
