#include "stdafx.h"
#include "InputValue.h"

bool InputValue::GetKeyword(const TCHAR* prompt, const TCHAR* keywords, TCHAR* result, size_t buffer_size)
{
	if (!prompt || !keywords || !result || buffer_size < 2)
	{
		return false;
	}
	acedInitGet(0, keywords);
	constexpr size_t INPUT_BUFFER_SIZE = 512;
	TCHAR input_buffer[INPUT_BUFFER_SIZE] = { 0 };
	int return_value = acedGetKword(prompt, input_buffer, INPUT_BUFFER_SIZE - 1);

	switch (return_value)
	{
	case RTNORM:
	{
		_tcscpy_s(result, buffer_size, input_buffer);
		return true;
	}
	case RTCAN:
	case RTNONE:
	default:
		*result = _T('\0');
		return false;
	}
}
