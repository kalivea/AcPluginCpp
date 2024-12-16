#pragma once
#include "StdAfx.h"
class POLADLL_GRID_API Grid
{
private:
	double circle_radius;
	double line_length;
	double axis_angle;
public:
	Grid(double radius, double length, double angle)
		:circle_radius(radius), line_length(length), axis_angle(angle)
	{
	}
	~Grid()
	{
	}

	AcDbObjectId CreateVerticalAxis();
	AcDbObjectId CreateHorizonAxis();

	static bool IsBlockExist(TCHAR* block_name);
	static AcDbObjectId GetBlockId(TCHAR* block_name);

	//static AcDbObjectId GetAttributeId(TCHAR* att_name);

	static void AddAttribute(AcDbBlockReference* block_reference, AcDbAttributeDefinition* att_definition);
	static void SetAttribute(AcDbBlockReference* block_reference, TCHAR* tag, TCHAR* att_string);

	static AcDbObjectId InsertVerticalAxis(AcGePoint3d insert_point);
	static AcDbObjectId InsertHorizonAxis(AcGePoint3d insert_point);

	static void DrawVerticalAxis(AcGePoint3dArray insert_point, TCHAR* insert_att[]);
	static void DrawHorizonAxis(AcGePoint3dArray insert_point, TCHAR* insert_att[]);
	//AcDbObjectId CreateHorizonAxis();

	//void DrawLogic();
};

