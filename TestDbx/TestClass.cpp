#include "StdAfx.h"
#include "TestClass.h"

void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestClass"), _T("TestClass"), ACRX_CMD_MODAL, Test);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}
void TestClass::Test()
{
#pragma region Grid 
	//AcGePoint3d c = AcGePoint3d(0, 0, 0);
	//AcGePoint3d c1 = AcGePoint3d(3500, 3500, 0);
	//AcGePoint3d c2 = AcGePoint3d(-2000, 7500, 0);
	//AcGePoint3d c3 = AcGePoint3d(1000, 0, 0);
	//StyleTools::CreateTextStyle(_T("grid_text"), _T("hztxt.shx"), _T("gbenor.shx"));
	//StyleTools::CreateTextStyle(_T("dim_text"), _T("hztxt.shx"), _T("gbenor.shx"));
	//AcDbObjectId dim_style_id = StyleTools::InitDimStyle();


	//Grid gg = Grid(500, 10000, 0);
	//gg.CreateVerticalAxis();
	//gg.CreateHorizonAxis();
	////horizon ex
	//double d[4] = { 9500,7500,8500,8500 };
	//TCHAR* att[5] = { L"1",L"2",L"3",L"4",L"5" };
	//AcGePoint3dArray hor_array = BasicTools::DistanceToPointArrayX(c1, d, 5);
	//Grid::DrawVerticalAxis(hor_array, att);
	//for (int i = 0; i < hor_array.length() - 1; i++)
	//{
	//	DrawEntity::AlignedDimension(hor_array[i], hor_array[i + 1], BasicTools::OffsetMidPoint(hor_array[i], hor_array[i + 1], 1700), dim_style_id);
	//}
	////vertical ex
	//double dd[4] = { 9500,7500,8500,8500 };
	//TCHAR* att2[5] = { L"A",L"B",L"C",L"D",L"E" };
	//AcGePoint3dArray ver_array = BasicTools::DistanceToPointArrayY(c2, dd, 5);
	//Grid::DrawHorizonAxis(ver_array, att2);
	//for (int i = 0; i < ver_array.length() - 1; i++)
	//{
	//	DrawEntity::AlignedDimension(ver_array[i], ver_array[i + 1], BasicTools::OffsetMidPoint(ver_array[i], ver_array[i + 1], 1700), dim_style_id);
	//}
#pragma endregion
#pragma region Line Type Test
	AcGePoint3d s_p(0, 0, 0);
	AcGePoint3d e_p(1000, 1000, 0);
	AcDbObjectId l_id = DrawEntity::DrawLine(s_p, e_p);

	AcGePoint3d s_p2(1000, 1000, 0);
	AcGePoint3d e_p2(2000, 2000, 0);
	AcDbObjectId l_id2 = DrawEntity::DrawLine(s_p2, e_p2);
	StyleTools::LoadLineType(_T("ACAD_ISO02W100"), _T("acad.lin"));

	AcDbEntity* l_ent = nullptr;
	acdbOpenAcDbEntity(l_ent, l_id, OpenMode::kForWrite);
	l_ent->setLinetype(_T("ACAD_ISO02W100"));
	l_ent->setColorIndex(1);
	l_ent->close();

	AcDbEntity* l_ent2 = nullptr;
	acdbOpenAcDbEntity(l_ent2, l_id2, OpenMode::kForWrite);
	l_ent2->setLinetype(StyleTools::GetLineStyleId(_T("ACAD_ISO02W100")));
	l_ent2->setColorIndex(2);
	l_ent2->close();
#pragma endregion


}