#pragma once
#include "dbmain.h"
#include <tchar.h>
#include "CommonDef.h"
#include <vector>
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
	// Borrows the transient entity on pre-append failure. appendAcDbEntity success
	// transfers ownership to the database; this helper then closes that handle.
	// Output is cleared on failure (including a close failure after transfer).
	// close_entity=false leaves the appended handle open for the caller to close.
	// Owners must use false, then inspect residency before releasing/closing;
	// with true, no caller may dereference entity_pointer after an append.
	static Acad::ErrorStatus AddEntityToModelSpace(AcDbEntity* entity_pointer, AcDbObjectId& entity_id, bool close_entity = true);
	// Legacy consuming wrappers: delete transient entities on append failure.
	// Already resident or borrowed entities must use neither consuming overload.
	static AcDbObjectId AddEntityToModelSpace(AcDbEntity* entity_pointer);
	static AcDbObjectIdArray AddEntityToModelSpace(std::vector<AcDbEntity*> entity_pointer_vector);
};
