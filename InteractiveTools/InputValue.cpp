#include "stdafx.h"
#include "InputValue.h"

bool InputValue::GetKeyword(const TCHAR* prompt, const TCHAR* keyword_buff, TCHAR* result_value)
{
	TCHAR keyword[128] = { 0 };
	acedInitGet(RSG_NONULL, keyword_buff);
	int ret = acedGetKword(prompt, keyword);
	switch (ret)
	{
	case RTNORM:
		acutPrintf(_T("Successfully obtained keywords!\n"));
		result_value = keyword;
		return true;
	case RTCAN:
		acutPrintf(_T("User cancel an operation!\n"));
		return false;
	default:
		return false;
	}
}
