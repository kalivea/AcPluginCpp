#pragma once
class SelectEntitys
{
public:
	static bool PickEntitys(const TCHAR* prompt, struct resbuf* result_buff, AcDbObjectIdArray& entity_id_array);
};

