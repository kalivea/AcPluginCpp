#include "stdafx.h"
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
#pragma region Test



	/*        //***Test Draw Line Method 1: Two Point Create a Line.***      //  Pass
	*
		AcGePoint3d start_point(0, 0, 0);
		AcGePoint3d end_point(100, 100, 0);
		DrawEntity::DrawLine(BasicTools::GetMidPoint(start_point, end_point), end_point);

	*/

	/*        //***Test Draw Line Method 1: Two Point Create a Line.***      // Pass
		AcGePoint3d start_point(0, 0, 0);
		AcGePoint3d end_point(100, 100, 0);
		DrawEntity::DrawLine(start_point, end_point);
	*/
	/*        //***Test Draw Line Method 2: Start Point, Angle to X-axis and Length Create a Line.***      // Pass

		AcGePoint3d start_point1(0, 10, 0);
		AcGePoint3d end_point1(10, 10, 0);
		DrawEntity::DrawLine(start_point1, end_point1);
		DrawEntity::DrawLine(end_point1, 10, M_PI/4, "RAD");
	*/
	/*         //***Test Draw Circle Method 1: Center Point, Radius Create a Circle.***      // Pass
		AcGePoint3d center_point(0, 0, 0);
		DrawEntity::DrawCircle(center_point, 10);
	*/
	/*			//***Test An Ran's Task and Change Color***									//Pass
		ads_point center_point;
		ads_real radius;
		acedGetPoint(nullptr, _T("\n Select center point:"), center_point);
		acedGetReal(_T("Input radius:"), &radius);
		EditEntity::SetColor(DrawEntity::DrawCircle(BasicTools::AdsPointToPoint3d(center_point), radius), 3);
	*/
	/*        //***Test Draw Circle Method 2: Two Point Create a Circle.***      // Pass
		AcGePoint3d start_point1(0, 0, 0);
		AcGePoint3d end_point1(10, 10, 0);
		DrawEntity::DrawCircle(start_point1, end_point1);
	*/

	/*         //***Test Draw Circle Method 3: Three Point Create a Circle.***      // Pass
		AcGePoint3d start_point1(0, 10, 0);
		AcGePoint3d end_point1(10, 10, 0);
		AcGePoint3d start_point2(0, 0, 0);
		DrawEntity::DrawCircle(start_point1, end_point1, start_point2);
	*/
	/*         //***Test Draw Arc Method 1: Three Point Create an Arc.***      // Pass
		AcGePoint3d start_point(100, 100, 0);
		AcGePoint3d point_on_arc(150, 100, 0);
		AcGePoint3d end_point(200, 200, 0);
		DrawEntity::DrawArc(start_point, point_on_arc, end_point);
	*/
	/*         //***Test Draw Arc Method 2: Center Point, Angle and Start Point Create an Arc.***      // Pass

		AcGePoint3d center_point(0, 0, 0);
		AcGePoint3d start_point(5, 5, 0);
		EditEntity::ChangeColor(DrawEntity::DrawArc(center_point, start_point, 45, "DEG"), 3);
		DrawEntity::DrawArc(center_point, start_point, -45, "DEG");
	*/
	/*         //***Test Move Entity Method.***      // Pass
		AcGePoint3d center_point(0, 0, 0);
		AcGePoint3d point(10, 10, 0);
		AcDbObjectId circle_id = DrawEntity::DrawCircle(center_point, 5);

		EditEntity::MoveEntity(circle_id, center_point, point);

	*/
	/*         //***Test Copy Entity Method.***      // Pass
	*/


	/*         //***Test Rotate Entity Method.***      // Pass

		AcGePoint3d start_point(0, 0, 0);
		AcGePoint3d end_point(10, 0, 0);
		AcDbObjectId line_id = DrawEntity::DrawLine(start_point, end_point);
		EditEntity::RotateEntity(line_id, AcGePoint3d::kOrigin, M_PI/4.0, "RAD");
	*/
	/*         //***Test Scale Entity Method.***      // Pass

		AcGePoint3d center_point(0, 0, 0);
		AcGePoint3d center_point2(10, 0, 0);
		AcDbObjectId circle_id = DrawEntity::DrawCircle(center_point, 5);
		EditEntity::ScaleEntity(circle_id, center_point2, 5);
		EditEntity::DeleteEntity(circle_id);
		*/
		/*         //***Test Mirror Entity Method.***      // Pass
		*
		AcGePoint3d center_point1(15, 0, 0);
		AcDbObjectId circle1_id = DrawEntity::DrawCircle(center_point1, 2.5);
		AcGePoint3d mirror1_point1(10, 0, 0);
		AcGePoint3d mirror1_point2(10, 10, 0);

		AcGePoint3d center_point2(-15, 0, 0);
		AcDbObjectId circle2_id = DrawEntity::DrawCircle(center_point2, 2.5);
		AcGePoint3d mirror2_point1(-10, 0, 0);
		AcGePoint3d mirror2_point2(-10, 10, 0);

		EditEntity::MirrorEntity(circle1_id, mirror1_point1, mirror1_point2, false);
		EditEntity::MirrorEntity(circle2_id, mirror2_point1, mirror2_point2, true);
	*/
	/* //***Test Class: Draw Standard Entrance Main Logic.***      // Pass
	  AcGePoint3d local_origin_point1(0, 0, 0);
	  AcGePoint3d local_origin_point2(0, 10000, 0);
	  AcGePoint3d local_origin_point3(0, 20000, 0);
	  AcGePoint3d local_origin_point4(0, 30000, 0);

	  StandardEntrance e1(local_origin_point1, 7200, 5400, 600, 700, 600, 0, 0, 0, 0);
	  StandardEntrance e2(local_origin_point2, 7200, 5400, 600, 700, 600, 1000, 500, 0, 0);
	  StandardEntrance e3(local_origin_point3, 7200, 5400, 600, 700, 600, 0, 0, 450, 300);
	  StandardEntrance e4(local_origin_point4, 7200, 5400, 600, 700, 600, 1000, 500, 450, 300);

	  e1.DrawLogic();
	  e2.DrawLogic();
	  e3.DrawLogic();
	  e4.DrawLogic();
*/
/*   //***Test Class: Draw U-style Entrance Main Logic.***      // Pass
  AcGePoint3d local_origin_point14(10000, 0, 0);
  AcGePoint3d local_origin_point24(10000, 10000, 0);
  UstyleEntrance u1(local_origin_point14, 7200, 4200, 600, 700, 0, 0);
  UstyleEntrance u2(local_origin_point24, 7200, 4200, 600, 700, 1000, 500);
  u1.DrawLogic();
  u2.DrawLogic();
*/
/*         //***Test Class: Add a Text Method.***      // Pass
	AcGePoint3d local_origin_point1(0, 0, 0);
	AcDbObjectId style = AcDbObjectId::kNull;
	DrawEntity::AddText(local_origin_point1, _T("test"), style);
	*/
	/*
		AcGePoint3d point1(0, 0, 0);
		AcGePoint3d point2(1, 1, 0);
		AcGePoint3d point3(0.5, 1.5, 0);
		AcDbObjectId rect_id = DrawEntity::DrawLine(point1, point2);

		DrawEntity::AlignedDimension(point1, point2, point3);
		*/
		/*         //***Test Class: Aligned Dimension.***      // Pass
		AcGePoint3d point1(0, 0, 0);
		AcGePoint3d point2(234, -190, 0);
		DrawEntity::DrawLine(point1, point2);
		DrawEntity::AlignedDimension(point1, point2, BasicTools::OffsetMidPoint(point1, point2, 50));
		*/
		//StyleTools::InitTextStyle();
		/*	         //***Test Class: Create a New Text Style.***      // Pass
			AcGePoint3d point1(0, 0, 0);
			AcDbObjectId txt_style_id = StyleTools::CreateTextStyle(_T("TTT"), _T("pt_hz.shx"), _T("gbenor.shx"));
			DrawEntity::AddText(point1, _T("Add test text!"), StyleTools::GetTextStyleId(_T("TTT")), 500, 0);
			*/
#pragma endregion
#pragma region Grid			//Test Pass

	AcGePoint3d c = AcGePoint3d(0, 0, 0);
	AcGePoint3d c1 = AcGePoint3d(3500, 3500, 0);
	AcGePoint3d c2 = AcGePoint3d(-2000, 7500, 0);
	AcGePoint3d c3 = AcGePoint3d(1000, 0, 0);
	StyleTools::CreateTextStyle(_T("grid_text"), _T("hztxt.shx"), _T("gbenor.shx"));
	StyleTools::CreateTextStyle(_T("dim_text"), _T("hztxt.shx"), _T("gbenor.shx"));
	AcDbObjectId dim_style_id = StyleTools::InitDimStyle();


	Grid::CreateHorizonAxis(500, 10000, 0);
	Grid::CreateVerticalAxis(500, 10000, 0);

	//horizon ex
	double d[4] = { 9500,7500,8500,8500 };
	TCHAR* att[5] = { L"1",L"2",L"3",L"4",L"5" };
	AcGePoint3dArray hor_array = BasicTools::DistanceToPointArrayX(c1, d, 5);
	Grid::DrawVerticalAxis(hor_array, att);
	for (int i = 0; i < hor_array.length() - 1; i++)
	{
		DrawEntity::AlignedDimension(hor_array[i], hor_array[i + 1], BasicTools::OffsetMidPoint(hor_array[i], hor_array[i + 1], 1700), dim_style_id);
	}
	//vertical ex
	double dd[4] = { 9500,7500,8500,8500 };
	TCHAR* att2[5] = { L"A",L"B",L"C",L"D",L"E" };
	AcGePoint3dArray ver_array = BasicTools::DistanceToPointArrayY(c2, dd, 5);
	Grid::DrawHorizonAxis(ver_array, att2);
	for (int i = 0; i < ver_array.length() - 1; i++)
	{
		DrawEntity::AlignedDimension(ver_array[i], ver_array[i + 1], BasicTools::OffsetMidPoint(ver_array[i], ver_array[i + 1], 1700), dim_style_id);
	}
#pragma endregion
#pragma region Pillar 

	/*AcGeLine3d line_1(AcGePoint3d(-100, 65, 0), AcGePoint3d(74, -69, 0));
	EditEntity::SetColor( DrawEntity::DrawLine(AcGePoint3d(-100, 65, 0), AcGePoint3d(74, -69, 0)),3);

	AcGeLine3d line_2(AcGePoint3d(-17, -675, 0), AcGePoint3d(36, 15, 0));
	EditEntity::SetColor(DrawEntity::DrawLine(AcGePoint3d(-17, -675, 0), AcGePoint3d(36, 15, 0)),1);

	AcGePoint3d aa;
	BasicTools::IsIntersectLine(line_1, line_2, aa);
	DrawEntity::DrawLine(aa, AcGePoint3d::kOrigin);*/
#pragma endregion

}