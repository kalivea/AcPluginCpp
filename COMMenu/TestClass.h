#pragma once
#include "resource.h"
#include "acadi_i.c"

class TestClass
{
public:
	static void TestClassInit();
	static void TestClassUnload();
	static void Test();
	void AddMenuItems(IAcadPopupMenu* pParentMenu, const MenuItem* items, int itemCount);
};
