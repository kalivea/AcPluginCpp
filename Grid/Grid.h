#pragma once
#include "dbxHeaders.h"
#include "GridDef.h"
#include <tchar.h>
class POLADLL_GRID_API Grid
{
private:
	double circle_radius_;
	double line_length_;
	double axis_angle_;
public:
	Grid(double radius, double length, double angle)
		:circle_radius_(radius), line_length_(length), axis_angle_(angle)
	{
	}
	~Grid()
	{
	}

	AcDbObjectId CreateVerticalAxis();
    Acad::ErrorStatus CreateVerticalAxis(AcDbObjectId& id);
	AcDbObjectId CreateHorizonAxis();
    Acad::ErrorStatus CreateHorizonAxis(AcDbObjectId& id);

	//static AcDbObjectId GetAttributeId(TCHAR* att_name);

	static Acad::ErrorStatus AddAttribute(AcDbBlockReference* block_reference, const AcDbAttributeDefinition* att_definition);
	static Acad::ErrorStatus SetAttribute(AcDbBlockReference* block_reference, const TCHAR* tag, const TCHAR* att_string);

	static Acad::ErrorStatus InsertVerticalAxis(const AcGePoint3d& point, AcDbObjectId& id);
	static AcDbObjectId InsertVerticalAxis(const AcGePoint3d& insert_point);
	static Acad::ErrorStatus InsertHorizonAxis(const AcGePoint3d& point, AcDbObjectId& id);
	static AcDbObjectId InsertHorizonAxis(const AcGePoint3d& insert_point);

	static Acad::ErrorStatus DrawVerticalAxis(const AcGePoint3dArray& insert_point, const TCHAR* insert_att[]);
	static Acad::ErrorStatus DrawHorizonAxis(const AcGePoint3dArray& insert_point, const TCHAR* insert_att[]);
	//AcDbObjectId CreateHorizonAxis();

	//void DrawLogic();
};
