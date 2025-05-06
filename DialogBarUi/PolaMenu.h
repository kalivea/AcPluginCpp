#pragma once
class PolaMenu
{
public:
	PolaMenu();
	virtual ~PolaMenu();

	void Set(CStringArray& menu, int& id);
	bool CreatePOP(CStringArray& menu, int& id, CMenu& sub_menu);
	bool CreateSUB(CStringArray& menu, int& id, CMenu& sub_menu);

public:
	CMenu menu_;
	AcGeIntArray id_;
	CStringArray command_;
};

