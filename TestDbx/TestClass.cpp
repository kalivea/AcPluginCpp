#include "StdAfx.h"
#include "TestClass.h"
#include "PolaReactor.h"
#include "PolaIRM.h"
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

	//AcGePoint3d st(0, 0, 0);
	//AcGePoint3d ed(100, 0, 0);
	//AcDbObjectId dim_id = DrawEntity::AlignedDimension(st, ed, BasicTools::OffsetMidPoint(st, ed, 30));
	//EditEntity::SetColor(dim_id, 1);
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
#pragma region ss--------------------------------------------
	//StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	//AcDbObjectIdArray line_array;
	//AcGePoint3dArray insert_point;
	//SelectEntitys::PickLinesOnLayer(_T("Line"), line_array);

	//AcDbEntity* entity = nullptr;
	//std::vector<AcGeLineSeg3d> line_vector;
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
#pragma region grid

	/*AcGePoint3d vertex_point1(0, 0, 0);
	AcGePoint3d vertex_point2(100, 100, 0);
	EditEntity::SetColor(DrawEntity::DrawRectangle(vertex_point1, vertex_point2, 0), 3);

	AcGePoint3d line_point1(0, 0, 0);
	AcGePoint3d line_point2(100, 100, 0);
	AcGePoint3d line_point3(20, -70, 0);
	AcGePoint3d line_point4(-70, 20, 0);

	if (BasicTools::IsLineThroughRectangle(line_point1, line_point2, vertex_point1, vertex_point2))
		acutPrintf(_T("is\n"));
	else
		acutPrintf(_T("!is\n"));
	if (BasicTools::IsLineThroughRectangle(line_point3, line_point4, vertex_point1, vertex_point2))
		acutPrintf(_T("is\n"));
	else
		acutPrintf(_T("!is\n"));

	DrawEntity::DrawLine(line_point1, line_point2);
	DrawEntity::DrawLine(line_point3, line_point4);*/

	//AcGePoint3d vertex_point1(0, 0, 0);
	//AcGePoint3d vertex_point2(100, 100, 0);
	//EditEntity::SetColor(DrawEntity::DrawRectangle(vertex_point1, vertex_point2, 0), 3);

	//AcGePoint3d line_point1(-10, -10, 0);
	//for (int i = 0; i < 360; i++)
	//{
	//	double line_point2_x = line_point1.x + 500 * cos(BasicTools::ConvertAngle(i, 1));
	//	double line_point2_y = line_point1.y + 500 * sin(BasicTools::ConvertAngle(i, 1));
	//	AcGePoint3d line_point2(line_point2_x, line_point2_y, 0);

	//	if (BasicTools::IsLineThroughRectangle(line_point1, line_point2, vertex_point1, vertex_point2))
	//	{
	//		EditEntity::SetColor(DrawEntity::DrawLine(line_point1, line_point2), 1);
	//	}
	//	else
	//	{
	//		EditEntity::SetColor(DrawEntity::DrawLine(line_point1, line_point2), 8);
	//	}
	//}
#pragma endregion
#pragma region Beam
//	StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
//	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
//
//	//------------------------------------------------------------------------
//	CPolaCustomBeam* beam = new CPolaCustomBeam();
//	beam->setBeamWidth(1200);
//	beam->setBeamHeight(1500);
//	beam->setBeamProperty(1);
//	//-------------------------------------------------------------------------
//	AcDbObjectId beam_id = CPolaCustomBeam::PickCenterPointDrawBeam(beam);
//	//CPolaCustomBeam::PickTopPointDrawBeam(beam);
////CPolaCustomBeam::PickBottomPointDrawBeam(beam);
//
//	AcDbObjectIdArray beam_array = beam->GetIntersectingPillar();
//	//acutPrintf(_T("Total %d pillar\n"), beam_array.length());
//	//EditEntity::SetColor(beam_array, 1);
//	//EditEntity::SetColor(beam_id, 2);
//	beam->addJoint(800);
//	beam->addBeamSnInfo();
//	acutPrintf(_T("beam length= %f\n"), beam->getBeamLength());
//	//---------------------------------------------------------------------------

#pragma endregion
#pragma region proj
	//AcGePoint3d start_point(0, 0, 0);
	//AcGePoint3d end_point(100, 100, 0);
	//AcGeLineSeg3d line_seg(start_point, end_point);

	//AcGePoint3d pt(80, 10, 0);
	//EditEntity::SetColor(DrawEntity::DrawLine(start_point, end_point), 8);
	//EditEntity::SetColor(DrawEntity::DrawLine(pt, BasicTools::ProjectPointToLineSeg(pt, line_seg, AcGeVector3d(0, 1, 0))), 1);
#pragma endregion
#pragma region pillar tools test
	//StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	//StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	//CPolaCustomPillar* pillar_test = new CPolaCustomPillar();
	//pillar_test->setCenterPoint(AcGePoint3d(100, 100, 0));
	//pillar_test->setPillarProperty(0);
	//pillar_test->setViewable(1);
	//pillar_test->setSn(1);
	//pillar_test->setPillarType(1);
	//pillar_test->setDiameter(1000, 1000);
	//CPolaCustomPillar::SingleInsert(*pillar_test, AcGePoint3d(100, 100, 0));

	//AcGePoint3d point;
	//SelectEntitys::PickPoint(_T("Click a point\n"), point);
	//AcDbObjectId pillar_id;
	//double d;
	//if (PillarTools::detectPillar(point, pillar_id, d))
	//{
	//	EditEntity::SetColor(pillar_id, 1);
	//}
	//else
	//	acutPrintf(_T("no"));

#pragma endregion
#pragma region 20250210

	//-------------------- Draw Beam -------------------------------------------
	//StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	//StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));

	//CPolaCustomBeam* top_beam = new CPolaCustomBeam();
	//top_beam->setBeamWidth(1200);
	//top_beam->setBeamHeight(1500);

	//CPolaCustomBeam* center_beam = new CPolaCustomBeam();
	//center_beam->setBeamWidth(1200);
	//center_beam->setBeamHeight(1500);

	//CPolaCustomBeam* bottom_beam = new CPolaCustomBeam();
	//bottom_beam->setBeamWidth(1200);
	//bottom_beam->setBeamHeight(1500);

	//AcDbObjectId top_beam_id = CPolaCustomBeam::PickTopPointDrawBeam(top_beam);
	//AcDbObjectId center_beam_id = CPolaCustomBeam::PickCenterPointDrawBeam(center_beam);
	//AcDbObjectId bottom_beam_id = CPolaCustomBeam::PickBottomPointDrawBeam(bottom_beam);

	//const auto addJoint = [](AcDbObjectId& beam_object_id)
	//	{
	//		AcDbEntity* beam_entity = nullptr;
	//		acdbOpenObject(beam_entity, beam_object_id, OpenMode::kForWrite);
	//		CPolaCustomBeam* beam = CPolaCustomBeam::cast(beam_entity);
	//		beam->addJoint(800,150);
	//		beam->close();
	//	};
	//addJoint(center_beam_id);

	//AcGePoint3d point;
	//SelectEntitys::PickPoint(_T("Click a point\n"), point);
	//AcDbObjectId pillar_id;
	//double d;
	//if (PillarTools::detectPillar(point, pillar_id, d))
	//{
	//	EditEntity::SetColor(pillar_id, 1);
	//}
	//else
	//	acutPrintf(_T("no"));
#pragma endregion

#pragma region insert point test

	////-------------------- Draw Beam -------------------------------------------
	/*StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));


	CPolaCustomBeam* center_beam = new CPolaCustomBeam();
	center_beam->setBeamWidth(300);
	center_beam->setBeamHeight(500);

	AcDbObjectId center_beam_id = CPolaCustomBeam::PickCenterPointDrawBeam(center_beam);

	AcGePoint3d point;
	SelectEntitys::PickPoint(_T("Select point: \n"), point);

	AcDbEntity* beam_entity = nullptr;
	acdbOpenObject(beam_entity, center_beam_id, OpenMode::kForWrite);
	CPolaCustomBeam* beam = CPolaCustomBeam::cast(beam_entity);

	beam->InsertVertex(point);
	beam->close();*/
	//--------------------------------------------------------------------
	/*StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	AcDbObjectId beam_id = CPolaCustomBeam::genbeam();

	CPolaCustomBeam::ModifyViewable(beam_id, 2, 0);*/

	//AcDbPolyline* pl = new AcDbPolyline();
	//for (int i = 0;i < 5;i++)
	//{
	//	pl->addVertexAt(i, AcGePoint2d(i * 500, 100));
	//}
	//AddToModelSpace::AddEntityToModelSpace(pl);
	//AcGePoint3dArray of_pt;
	//BasicTools::OffsetPolyLine(*pl, 300, of_pt);
	//EditEntity::SetColor(DrawEntity::DrawPolyLine(BasicTools::Point3dToPoint2d(of_pt), 0, 0), 1);


	//CPolaCustomBeam* beam = new CPolaCustomBeam();
	//beam->setBeamWidth(1200);
	//beam->setBeamHeight(1500);

	//beam->SelectPillarDrawBeam(beam);

	//AcDbObjectIdArray pillar;
	//SelectEntitys::GetAllEntitysByType(CPolaCustomPillar::desc(), pillar);

	//const auto addText = [](AcDbObjectIdArray& pillar_array)
	//	{
	//		AcDbEntity* pillar = nullptr;
	//		for (int i = 0;i < pillar_array.length();i++)
	//		{
	//			std::wstring info;
	//			std::wstringstream info_stream;
	//			info_stream << i;
	//			info = info_stream.str();
	//			acdbOpenObject(pillar, pillar_array.at(i), OpenMode::kForRead);
	//			CPolaCustomPillar* p = CPolaCustomPillar::cast(pillar);
	//			DrawEntity::AddText(p->getCenterPoint(), info.c_str(), StyleTools::InitTextStyle(), 350);
	//			p->close();
	//		}
	//		pillar->close();
	//	};
	//addText(pillar);
#pragma endregion
#pragma region beam plus test----------------- modify viewable
	/*StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));*/
	//
	//	CPolaCustomBeam* beam = new CPolaCustomBeam();
	//	beam->setBeamWidth(1200);
	//	beam->setBeamHeight(1500);
	//	AcDbObjectId id = CPolaCustomBeam::SelectPillarDrawBeam(beam);
	//
	//	AcGePoint3d point;
	//	while (SelectEntitys::PickPoint(_T("Select point: \n"), point))
	//	{
	//		AcDbEntity* entity = nullptr;
	//		acdbOpenObject(entity, id, OpenMode::kForWrite);
	//		CPolaCustomBeam* beam_t = CPolaCustomBeam::cast(entity);
	//		CPolaCustomBeam::ModifyViewable(beam_t, beam_t->GetSegmentIndexByYProjection(point) + 1, 0);
	//		beam_t->close();
	//	}
#pragma endregion

#pragma region reacter
	//AcDbObjectIdArray circ_ids;
	//if (!SelectEntitys::PickEntitys(_T("select two objects to sync colors: "), AcDbCircle::desc(), circ_ids))
	//	return;

	//if (circ_ids.length() != 2)
	//{
	//	acutPrintf(_T("only can select two objects!"));
	//	return;
	//}

	//// Set up dictionary for storing reactors
	//AcDbDictionary* name_dict = nullptr;
	//Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()
	//	->getNamedObjectsDictionary(name_dict, OpenMode::kForWrite);
	//if (es != Acad::eOk)
	//{
	//	acutPrintf(_T("\nError getting named objects dictionary: %s"), acadErrorStatusText(es));
	//	return;
	//}

	//// First, create both reactors without attaching them
	//CPolaReactor* radius_reactor1 = new CPolaReactor(circ_ids.at(1), 1);
	//AcDbObjectId reactor1_id;
	//es = name_dict->setAt(_T("color_reactor1"), radius_reactor1, reactor1_id);
	//if (es != Acad::eOk)
	//{
	//	acutPrintf(_T("\nError storing reactor1: %s"), acadErrorStatusText(es));
	//	delete radius_reactor1;
	//	name_dict->close();
	//	return;
	//}
	//radius_reactor1->close();

	//CPolaReactor* radius_reactor2 = new CPolaReactor(circ_ids.at(0), 1);
	//AcDbObjectId reactor2_id;
	//es = name_dict->setAt(_T("color_reactor2"), radius_reactor2, reactor2_id);
	//if (es != Acad::eOk)
	//{
	//	acutPrintf(_T("\nError storing reactor2: %s"), acadErrorStatusText(es));
	//	delete radius_reactor2;
	//	name_dict->close();
	//	return;
	//}
	//radius_reactor2->close();

	//// Then, in a separate step, add the reactors to the circles
	//AcDbCircle* circ1 = nullptr;
	//es = acdbOpenObject(circ1, circ_ids[0], OpenMode::kForWrite);
	//if (es == Acad::eOk)
	//{
	//	circ1->addPersistentReactor(reactor1_id);
	//	circ1->close();
	//}
	//else
	//{
	//	acutPrintf(_T("\nError opening circle1: %s"), acadErrorStatusText(es));
	//}

	//AcDbCircle* circ2 = nullptr;
	//es = acdbOpenObject(circ2, circ_ids[1], OpenMode::kForWrite);
	//if (es == Acad::eOk)
	//{
	//	circ2->addPersistentReactor(reactor2_id);
	//	circ2->close();
	//}
	//else if (es == Acad::eWasNotifying)
	//{
	//	acutPrintf(_T("\nCircle2 is currently notifying - will try again later"));
	//}
	//else
	//{
	//	acutPrintf(_T("\nError opening circle2: %s"), acadErrorStatusText(es));
	//}

	//name_dict->close();
#pragma endregion
#pragma region pillar tools
	//AcGePoint3d point;
	//while (SelectEntitys::PickPoint(_T("pick point:"), point))
	//{
	//	AcDbObjectId pillar_id;
	//	double d;
	//	if (PillarTools::detectRoundPillar(point, pillar_id, d))
	//	{
	//		EditEntity::SetColor(pillar_id, 1);
	//		acutPrintf(_T("round pillar d =%f\n"), d);
	//	}
	//	else
	//	{
	//		acutPrintf(_T("no pillar\n"));
	//	}
	//}
#pragma endregion

#pragma region IRM
	//StyleTools::CreateTextStyle(_T("IRM_default"), _T("hztxt.shx"), _T("tssdeng.shx"));
	//PolaIRM irm;
	//irm.setBeamInfo(1, 23, 1200, 1000);
	//irm.setStirrupReinforcementInfo(14, 100, 6);
	//irm.setMainReinforcementInfo(18, 32, 18, 32);
	//irm.setSideReinforcementInfo(10, 25);
	//irm.DrawPolaIRM();
#pragma endregion
	/*AcDbObjectIdArray beam_id;
	SelectEntitys::PickEntitys(_T("Select beam:"), CPolaCustomBeam::desc(), beam_id);

	AcDbObjectPointer<CPolaCustomBeam> beam(beam_id.at(0), OpenMode::kForWrite);

	AcDbObjectIdArray pillar_id;
	PillarTools::GetAllPillar(pillar_id);
	for (auto& id : pillar_id)
	{
		int i = 0;
		AcDbObjectPointer<CPolaCustomPillar> pillar(id, OpenMode::kForRead);
		AcGePoint3d point = pillar->getCenterPoint();
		if (BeamTools::IsPointInsideBeam(beam, point))
		{
			acutPrintf(_T("Point[%d]: (%.3f,%.3f,%.3f)"),i,point.x,point.y,point.z);
			DrawEntity::AddText(point, _T("A"), StyleTools::InitTextStyle(), 350);
		}
		i++;
	}*/
	//AcDbObjectIdArray line;
	//SelectEntitys::PickEntitys(_T("polyline"), AcDbPolyline::desc(), line);


	//AcDbObjectPointer<AcDbPolyline> line1(line.at(0), OpenMode::kForWrite);
	//AcDbObjectPointer<AcDbPolyline> line2(line.at(1), OpenMode::kForWrite);

	////AcGePoint3dArray center = BasicTools::GetPolyLineIntersections(BasicTools::GetPolylineVertex(*line1), BasicTools::GetPolylineVertex(*line2));
	//AcGePoint3dArray center = BasicTools::GetPolyLineIntersections(*line1, *line2);
	//for (auto& p : center)
	//{
	//	EditEntity::SetColor(DrawEntity::DrawCircle(p, 30), 2);
	//}
	
}
