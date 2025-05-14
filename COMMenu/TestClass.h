#pragma once
#include "resource.h"
#include "acadi_i.c"

struct MenuItem {
	const char* name;          // �˵�������
	const char* macro;         // �������Ϊ�����ʾΪ�Ӳ˵�
	const MenuItem* subItems;  // �Ӳ˵�������
	int subItemCount;          // �Ӳ˵�������
};

class TestClass
{
public:
	static void TestClassInit();
	static void TestClassUnload();
	static void Test();
	static void AddMenuItems(IAcadPopupMenu* pParentMenu, const MenuItem* items, int itemCount);
};
