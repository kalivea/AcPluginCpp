#pragma once
#include "def.h"
#define POLA_DLL
/*
*
* Update Time: 2024-12-04
* This is a Commonly used CAD functions.
* It can post CAD drawable entity to modelspace.
*
*/
class POLADLL AddToModelSpace
{
public:
	static AcDbObjectId AddEntityToModelSpace(AcDbEntity* entity_pointer);
	//static AcDbObjectIdArray AddEntityToModelSpace(AcDbEntity* entity_pointer[]);		//TODO: Add entity_pointer array
};

