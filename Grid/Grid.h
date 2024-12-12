#pragma once
#define POLA_DLL
#include "def.h"
class POLADLL Grid
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

	static AcDbObjectId CreateVerticalAxis(double radius, double length, double angle);
	static AcDbObjectId CreateHorizonAxis(double radius, double length, double angle);
	 AcDbObjectId T();
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

