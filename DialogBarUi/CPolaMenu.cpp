#include "stdafx.h"
#include "CPolaMenu.h"

void AnalyzeData(CStringArray& saDatas, CString csString, CString csDivideStr)
{
	saDatas.RemoveAll();
	//	csString.Remove(' ');
	if (csString.GetLength() == 0) return;

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

bool CPolaMenu::CreatePOP(CStringArray& saMenu, int& nID, CMenu& subMenu)
{
	subMenu.CreatePopupMenu();
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
			if (CreateSUB(saMenu, nID, subMenu2))
				subMenu.AppendMenu(MF_POPUP, (UINT)subMenu2.m_hMenu, csStr);
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
	subMenu.CreatePopupMenu();
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
			if (CreateSUB(saMenu, nID, subMenu2))
				subMenu.AppendMenu(MF_POPUP, (UINT)subMenu2.m_hMenu, csStr);
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
	CStdioFile file;
	CFileException ex;

	if (!file.Open(filePath, CFile::modeRead | CFile::typeText, &ex))
		return false;

	saMenu.RemoveAll();

	CString csLine;

	while (file.ReadString(csLine))
	{
		csLine.Trim();
		if (!csLine.IsEmpty())
			saMenu.Add(csLine);
	}
	file.Close();
	return true;
}
