#pragma once
class SelectEntitys
{
public:
	static bool PickEntitys(const TCHAR* prompt, struct resbuf* result_buff, AcDbObjectIdArray& entity_id_array);		
	static bool PickEntitys(const TCHAR* prompt, const std::vector<AcRxClass*>& class_describe, AcDbObjectIdArray& entity_id_array);
	static bool PickLinesOnLayer(const TCHAR* layer_name, AcDbObjectIdArray& entity_id_array);
};

