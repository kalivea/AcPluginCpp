#include "stdafx.h"
#include "SelectEntitys.h"

bool SelectEntitys::PickEntitys(const TCHAR* prompt, resbuf* result_buff, AcDbObjectIdArray& entity_id_array)
{
	entity_id_array.removeAll();
	entity_id_array.setLogicalLength(0);

	bool result = false;
	ads_name selection_set_name;
	int result_status = acedSSGet(nullptr, nullptr, nullptr, result_buff, selection_set_name);

	if (result_status==RTNORM)
	{
		int length = 0;
		acedSSLength(selection_set_name, &length);
		for (int i = 0; i < length; i++)
		{
			ads_name entity_name;
			acedSSName(selection_set_name, i, entity_name);
			AcDbObjectId entity_id = AcDbObjectId::kNull;
			acdbGetObjectId(entity_id, entity_name);
			entity_id_array.append(entity_id);
		}
		result = true;
	}
	else
	{
		result = false;
	}
	acedSSFree(selection_set_name);
	return result;
}
