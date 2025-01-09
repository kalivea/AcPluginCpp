#pragma once
#include "StdAfx.h"
/*
*
* Update Time: 2024-12-04
* This is a Commonly used CAD functions.
* It can post CAD drawable entity to modelspace.
*
*/
class POLADLL_COMMON_API AddToModelSpace
{
public:
	static AcDbObjectId AddEntityToModelSpace(AcDbEntity* entity_pointer);
	static AcDbObjectIdArray AddEntityToModelSpace(std::vector<AcDbEntity*> entity_pointer_vector);		
};

