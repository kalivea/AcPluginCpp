#pragma once
class CPolaMenu
{
public:
	CPolaMenu();
	virtual ~CPolaMenu();

	void Set(CStringArray& saMenu, int& nID);
	bool CreatePOP(CStringArray& saMenu, int& nID, CMenu& subMenu);
	bool CreateSUB(CStringArray& saMenu, int& nID, CMenu& subMenu);
public:
	CMenu m_menu;

	AcGeIntArray m_saID;
	CStringArray m_saCommand;
};

