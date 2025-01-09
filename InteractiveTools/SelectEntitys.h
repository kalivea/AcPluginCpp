#pragma once
#include "StdAfx.h"
class POLADLL_INTERACTIVE_API SelectEntitys
{
public:
	static bool PickEntitys(const TCHAR* prompt, struct resbuf* result_buff, AcDbObjectIdArray& entity_id_array);
	static bool PickEntitys(const TCHAR* prompt, const std::vector<AcRxClass*>& class_describe, AcDbObjectIdArray& entity_id_array);
	static bool PickEntitys(const TCHAR* prompt, AcRxClass* class_describe, AcDbObjectIdArray& entity_id_array);
	static bool PickLinesOnLayer(const TCHAR* layer_name, AcDbObjectIdArray& entity_id_array);
	static bool PickEntitysInRectangel(const TCHAR* prompt, const AcGePoint2d& point1, const AcGePoint2d& point2, AcDbObjectIdArray& entity_id_array);
};

