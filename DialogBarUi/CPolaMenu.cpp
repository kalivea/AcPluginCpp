#include "stdafx.h"
#include "CPolaMenu.h"

void AnalyzeData(CStringArray& saDatas, CString csString, CString csDivideStr)
{
	saDatas.RemoveAll();
	//	csString.Remove(' ');
	if (csString.GetLength() == 0) return;
	if (csDivideStr.IsEmpty()) { saDatas.Add(csString); return; }

	int nPos = 0, nLastPos = 0;
	while ((nPos = csString.Find(csDivideStr, nLastPos)) != -1)
	{
		saDatas.Add(csString.Mid(nLastPos, nPos - nLastPos));
		nLastPos = nPos + csDivideStr.GetLength();
	}
	if (nPos == -1)
	{
		saDatas.Add(csString.Mid(nLastPos, csString.GetLength() - nLastPos));
	}
}

CPolaMenu::CPolaMenu()
{
}

CPolaMenu::~CPolaMenu()
{
}

void CPolaMenu::Set(CStringArray& saMenu, int& nID)
{
	bool isNewMenu = false;
	if (m_menu.GetSafeHmenu() != nullptr) m_menu.DestroyMenu();
	m_saCommand.RemoveAll();
	m_saID.removeAll();
	if (!m_menu.CreateMenu()) return;
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
			if (CreatePOP(saMenu, nID, subMenu) && m_menu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(subMenu.GetSafeHmenu()), csStr))
				subMenu.Detach(); // The parent menu now owns this native submenu.
		}
	}
}

bool CPolaMenu::CreatePOP(CStringArray& saMenu, int& nID, CMenu& subMenu)
{
	if (!subMenu.CreatePopupMenu()) return false;
	while (saMenu.GetSize() > 0)
	{
		CString csStr = saMenu[0];
		csStr.Replace(_T(" "), _T(""));
		csStr.Replace(_T("	"), _T(""));
		if (_T("") == csStr)
		{
			saMenu.RemoveAt(0);
			continue;
		}
		else if (_T("***POP") == csStr.Left(6))
		{
			return true;
		}
		else if (_T("[--]") == csStr.Left(4))
		{
			saMenu.RemoveAt(0);
			subMenu.AppendMenu(MF_SEPARATOR);
		}
		else if (_T("[->") == csStr.Left(3))
		{
			csStr = CString(saMenu[0]);
			csStr.TrimLeft();
			saMenu.RemoveAt(0);
			csStr.Replace(_T("[->"), _T(""));
			csStr.Replace(_T("]"), _T(""));
			CMenu subMenu2;
			if (CreateSUB(saMenu, nID, subMenu2) && subMenu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(subMenu2.GetSafeHmenu()), csStr))
				subMenu2.Detach();
		}
		else
		{
			csStr = CString(saMenu[0]);
			csStr.TrimLeft();
			saMenu.RemoveAt(0);
			CStringArray saDatas;
			AnalyzeData(saDatas, csStr, _T("]"));
			if (2 == saDatas.GetSize())
			{
				CString csName = saDatas[0];
				csName.Replace(_T("["), _T(""));
				CString csCmd = saDatas[1];
				subMenu.AppendMenu(MF_STRING, ++nID, csName);
				m_saCommand.Add(csCmd);
				m_saID.append(nID);
			}
		}
	}
	return true;
}

bool CPolaMenu::CreateSUB(CStringArray& saMenu, int& nID, CMenu& subMenu)
{
	if (!subMenu.CreatePopupMenu()) return false;
	while (saMenu.GetSize() > 0)
	{
		CString csStr = saMenu[0];
		csStr.TrimLeft();
		if (_T("") == csStr)
		{
			saMenu.RemoveAt(0);
			continue;
		}
		else if (_T("[<-") == csStr.Left(3))
		{
			saMenu.RemoveAt(0);
			CStringArray saDatas;
			AnalyzeData(saDatas, csStr, _T("]"));
			if (2 == saDatas.GetSize())
			{
				CString csName = saDatas[0];
				csName.Replace(_T("[<-"), _T(""));
				CString csCmd = saDatas[1];
				subMenu.AppendMenu(MF_STRING, ++nID, csName);
				m_saCommand.Add(csCmd);
				m_saID.append(nID);
			}
			return true;
		}
		else if (_T("[--]") == csStr.Left(4))
		{
			saMenu.RemoveAt(0);
			subMenu.AppendMenu(MF_SEPARATOR);
		}
		else if (_T("[->") == csStr.Left(3))
		{
			saMenu.RemoveAt(0);
			csStr.Replace(_T("[->"), _T(""));
			csStr.Replace(_T("]"), _T(""));
			CMenu subMenu2;
			if (CreateSUB(saMenu, nID, subMenu2) && subMenu.AppendMenu(MF_POPUP, reinterpret_cast<UINT_PTR>(subMenu2.GetSafeHmenu()), csStr))
				subMenu2.Detach();
		}
		else
		{
			saMenu.RemoveAt(0);
			CStringArray saDatas;
			AnalyzeData(saDatas, csStr, _T("]"));
			if (2 == saDatas.GetSize())
			{
				CString csName = saDatas[0];
				csName.Replace(_T("["), _T(""));
				CString csCmd = saDatas[1];
				subMenu.AppendMenu(MF_STRING, ++nID, csName);
				m_saCommand.Add(csCmd);
				m_saID.append(nID);
			}
		}
	}
	return true;
}

bool CPolaMenu::ReadMenuConfigFromFile(const CString& filePath, CStringArray& saMenu)
{
	saMenu.RemoveAll();
	CStdioFile file;
	CFileException ex;

	if (!file.Open(filePath, CFile::modeRead | CFile::typeText, &ex))
		return false;

	CStringArray loaded;
	try
	{
		CString csLine;
		while (file.ReadString(csLine))
		{
			csLine.Trim();
			if (!csLine.IsEmpty()) loaded.Add(csLine);
		}
		file.Close();
	}
	catch (CFileException* error)
	{
		error->Delete();
		return false;
	}
	saMenu.Copy(loaded);
	return true;
}
