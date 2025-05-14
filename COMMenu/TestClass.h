#pragma once
#include "resource.h"
#include "acadi_i.c"

struct MenuItem {
	const char* name;          // 菜单项名称
	const char* macro;         // 宏命令，若为空则表示为子菜单
	const MenuItem* subItems;  // 子菜单项数组
	int subItemCount;          // 子菜单项数量
};

class TestClass
{
public:
	static void TestClassInit();
	static void TestClassUnload();
	static void Test();
	static void AddMenuItems(IAcadPopupMenu* pParentMenu, const MenuItem* items, int itemCount);
};
