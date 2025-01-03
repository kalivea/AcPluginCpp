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
	//AcGePoint3d s_p(0, 0, 0);
	//AcGePoint3d e_p(1000, 1000, 0);
	//AcDbObjectId l_id = DrawEntity::DrawLine(s_p, e_p);

	//AcGePoint3d s_p2(1000, 1000, 0);
	//AcGePoint3d e_p2(2000, 2000, 0);
	//AcDbObjectId l_id2 = DrawEntity::DrawLine(s_p2, e_p2);
	//StyleTools::LoadLineType(_T("ACAD_ISO02W100"), _T("acad.lin"));

	//AcDbEntity* l_ent = nullptr;
	//acdbOpenAcDbEntity(l_ent, l_id, OpenMode::kForWrite);
	//l_ent->setLinetype(_T("ACAD_ISO02W100"));
	//l_ent->setColorIndex(1);
	//l_ent->close();

	//AcDbEntity* l_ent2 = nullptr;
	//acdbOpenAcDbEntity(l_ent2, l_id2, OpenMode::kForWrite);
	//l_ent2->setLinetype(StyleTools::GetLineStyleId(_T("ACAD_ISO02W100")));
	//l_ent2->setColorIndex(2);
	//l_ent2->close();

/*AcGePoint3d vertex_array[4];
	AcGePoint3d center_point(0, 0, 0);
	AcGeVector3d dir(sqrt(3), 1, 0);
	AcGePoint2dArray _2d;
	AcGePoint2dArray _2d1;
	double pillar_d = 800;
	double pillar_h = 1000;*/

#pragma endregion
#pragma region DBX TEST

	//StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	//AcGePoint3dArray insert_point;
	////-----------------------------------------------------------------
	//CPolaCustomPillar* pillar = new CPolaCustomPillar();
	//pillar->setDiameter(350, 500);
	//pillar->setPillarType(0);
	//pillar->setViewable(true);
	//pillar->setDirectionVector();
	//pillar->setPillarProperty(0);
	//pillar->setSn(1);
	//insert_point.removeAll();
	//for (int i = 0; i < 5; i++)
	//{
	//	insert_point.append(AcGePoint3d(i * 2500, 1000, 0));
	//}
	//CPolaCustomPillar::BatchInsert(*pillar, insert_point);
	////------------------------------------------------------------------
	//CPolaCustomPillar* pillar2 = new CPolaCustomPillar();
	//pillar2->setDiameter(350, 500);
	//pillar2->setPillarType(1);
	//pillar2->setViewable(true);
	//pillar2->setDirectionVector(AcGeVector3d(1, 1, 0));
	//pillar2->setPillarProperty(0);
	//pillar2->setSn(1);
	//insert_point.removeAll();
	//for (int i = 0; i < 5; i++)
	//{

	//	insert_point.append(AcGePoint3d(i * 2500, 0, 0));
	//}
	//CPolaCustomPillar::BatchInsert(*pillar2, insert_point);
#pragma endregion
#pragma region MFC Test
	//AcGePoint3d p1(0, 0, 0);
	//AcGePoint3d p2(100, 100, 0);
	//AcGePoint3d p3(200, 300, 0);
	//if (BasicTools::IsCollinearPoint(p1, p2, p3))
	//	acutPrintf(_T("is"));
	//else
	//	acutPrintf(_T("!is"));
	AcGePoint3d insert(-10, 0, 0);
	AcGePoint3d last(20, 20, 0);
	AcGePoint3d text(20.5, 20.5, 0);

	StyleTools::CreateTextStyle(_T("dim_text"), _T("gbcbig.shx"), _T("gbenor.shx"));

	DrawEntity::AddMLeader(insert, last, text, _T("Test\n350*500"));
	
	//StyleTools::CreateLayerStyle(_T("TEST"), 1, _T("Continuous"));
	//
	//EditEntity::SetLayer(DrawEntity::DrawCircle(AcGePoint3d(0, 0, 0), 5), _T("TEST"));
	//DrawEntity::DrawLine(AcGePoint3d(0, 0, 0), 5, 45, 0);
	//DrawEntity::DrawArc(AcGePoint3d(10, 10, 0), AcGePoint3d(20, 10, 0), 45, 0);


	//AcDbObjectIdArray hatch_arrary;

	//hatch_arrary.append(DrawEntity::DrawCircle(AcGePoint3d(0, 0, 0), 10));

	//DrawEntity::HatchPattern(_T("ANGLE"), 0.5, 45, 0, true, hatch_arrary);


#pragma endregion
}