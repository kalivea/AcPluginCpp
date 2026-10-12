#pragma once
#include "dbxHeaders.h"
#include "InteractiveDef.h"
#include <tchar.h>
#include <vector>
class POLADLL_INTERACTIVE_API SelectEntitys
{
public:
    static Acad::ErrorStatus GetAllEntitysByTypeChecked(const AcRxClass* type, AcDbObjectIdArray& ids);
	// select entitys
    static Acad::ErrorStatus PickEntitysChecked(const TCHAR* prompt, struct resbuf* filter, AcDbObjectIdArray& ids);
    static Acad::ErrorStatus PickEntitysChecked(const TCHAR* prompt, const std::vector<AcRxClass*>& classes, AcDbObjectIdArray& ids);
    static Acad::ErrorStatus PickEntitysInRectangelChecked(const TCHAR* prompt, const AcGePoint2d& first, const AcGePoint2d& second, AcDbObjectIdArray& ids);
	static bool PickEntitys(const TCHAR* prompt, struct resbuf* result_buff, AcDbObjectIdArray& entity_id_array);
	static bool PickEntitys(const TCHAR* prompt, const std::vector<AcRxClass*>& class_describe, AcDbObjectIdArray& entity_id_array);
	static bool PickEntitys(const TCHAR* prompt, AcRxClass* class_describe, AcDbObjectIdArray& entity_id_array);
	static bool PickLinesOnLayer(const TCHAR* layer_name, AcDbObjectIdArray& entity_id_array);
	static bool PickEntitysInRectangel(const TCHAR* prompt, const AcGePoint2d& point1, const AcGePoint2d& point2, AcDbObjectIdArray& entity_id_array);

	// select points
	static bool PickPoint(const TCHAR* prompt, AcGePoint3d& point);
	static bool PickPoint(const TCHAR* prompt, const AcGePoint3d& base_point, AcGePoint3d& point);

	//
	static bool GetAllEntitysByType(const AcRxClass* class_describe, AcDbObjectIdArray& entity_id_array);
};
