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
	//AcGePoint3d insert(-10, 0, 0);
	//AcGePoint3d last(20, 20, 0);
	//AcGePoint3d text(20.5, 20.5, 0);

	//StyleTools::CreateTextStyle(_T("dim_text"), _T("gbcbig.shx"), _T("gbenor.shx"));

	//DrawEntity::AddMLeader(insert, last, text, _T("Test\n350*500"));

	//StyleTools::CreateLayerStyle(_T("TEST"), 1, _T("Continuous"));
	//
	//EditEntity::SetLayer(DrawEntity::DrawCircle(AcGePoint3d(0, 0, 0), 5), _T("TEST"));
	//DrawEntity::DrawLine(AcGePoint3d(0, 0, 0), 5, 45, 0);
	//DrawEntity::DrawArc(AcGePoint3d(10, 10, 0), AcGePoint3d(20, 10, 0), 45, 0);


	//AcDbObjectIdArray hatch_arrary;

	//hatch_arrary.append(DrawEntity::DrawCircle(AcGePoint3d(0, 0, 0), 10));

	//DrawEntity::HatchPattern(_T("ANGLE"), 0.5, 45, 0, true, hatch_arrary);


#pragma endregion
	/*EditEntity::SetColor(DrawEntity::DrawArc(AcGePoint3d(900, 500, 0), AcGePoint3d(480, 690, 0), AcGePoint3d(0, 0, 0)), 1);
	EditEntity::SetColor(DrawEntity::DrawArc(AcGePoint3d(0, 0, 0), AcGePoint3d(400, 400, 0), 45, 0), 2);
	EditEntity::SetColor(DrawEntity::DrawArc(AcGePoint3d(100, 100, 0), AcGePoint3d(200, 100, 0), AcGePoint3d(0, 100, 0), 1), 3);*/
	//EditEntity::SetColor(DrawEntity::DrawPolyLine(BasicTools::DistanceToPointArrayX(AcGePoint3d(0, 0, 0), 100, 10), 0.5, true), 1);

#pragma region ss
	//StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	//AcDbObjectIdArray line_array;
	//AcGePoint3dArray insert_point;
	//SelectEntitys::PickLinesOnLayer(_T("Line"), line_array);

	//AcDbEntity* entity = nullptr;
	//std::vector<AcGeLine3d> line_vector;
	//for (int i = 0; i < line_array.length(); i++)
	//{
	//	if (acdbOpenObject(entity, line_array.at(i), AcDb::kForRead) == Acad::eOk)
	//	{
	//		line_vector.push_back(BasicTools::EntityToLine(entity));
	//	}
	//}

	//for (int i = 0; i < line_vector.size(); i++)
	//{
	//	for (int j = i + 1; j < line_vector.size(); j++)
	//	{
	//		if (BasicTools::GetIntersect(line_vector.at(i), line_vector.at(j)) != AcGePoint3d(6496,6496,6496))
	//		{
	//			insert_point.append(BasicTools::GetIntersect(line_vector.at(i), line_vector.at(j)));
	//		}
	//	}
	//}
	//CPolaCustomPillar* pillar = new CPolaCustomPillar();
	//pillar->setDiameter(350, 500);
	//pillar->setSn(1);
	//pillar->setPillarProperty(1);
	//pillar->setPillarType(1);
	//pillar->setViewable(true);

	//CPolaCustomPillar::BatchInsert(*pillar, insert_point);

#pragma endregion
#pragma region ss plus

	//AcGePoint3d rect_point1(0, 0, 0);
	//AcGePoint3d rect_point2(100, 100, 0);

	//AcGePoint3d rect_point3(50, 50, 0);
	//AcGePoint3d rect_point4(250, 250, 0);

	//AcGePoint3d rect_point5(-100, 150, 0);
	//AcGePoint3d rect_point6(-70, 0, 0);

	//AcGePoint3d rect_point7(0, 0, 0);
	//AcGePoint3d rect_point8(-100, -100, 0);
	/*
	if (BasicTools::IsIntersectRectangle(rect_point1, rect_point2, rect_point3, rect_point4))
		acutPrintf(_T("is\n"));
	else
		acutPrintf(_T("!is\n"));

	if (BasicTools::IsIntersectRectangle(rect_point1, rect_point2, rect_point5, rect_point6))
		acutPrintf(_T("is\n"));
	else
		acutPrintf(_T("!is\n"));

	if (BasicTools::IsIntersectRectangle(rect_point1, rect_point2, rect_point7, rect_point8))
		acutPrintf(_T("is\n"));
	else
		acutPrintf(_T("!is\n"));*/

		/*AcDbObjectIdArray ent = BasicTools::GetAllEntityIdsInDatabase();
		acutPrintf(_T("%d\n"), ent.length());*/
	//DrawEntity::DrawRectangle(rect_point1, rect_point2, 0);
	//DrawEntity::DrawRectangle(rect_point3, rect_point4, 0);
	//DrawEntity::DrawRectangle(rect_point5, rect_point6, 0);
	//DrawEntity::DrawRectangle(rect_point7, rect_point8, 0);

	////EditEntity::SetColor(DrawEntity::DrawRectangle(AcGePoint3d(-500, 500, 0), AcGePoint3d(500, -500, 0), 0), 2);
	//AcDbObjectIdArray ent;

	//SelectEntitys::PickEntitysInRectangel(NULL, AcGePoint2d(-499, 499), AcGePoint2d(499, -499), ent);
	//acutPrintf(_T("%d\n"), ent.length());
	//
	//EditEntity::SetColor(ent, 3);
	//EditEntity::SetLayer(ent, _T("TEST"));
	//EditEntity::SetLinetype(ent, _T("DASHED"),20);

#pragma endregion

}