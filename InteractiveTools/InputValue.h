#pragma once
class POLADLL_INTERACTIVE_API InputValue
{
public:
	static bool GetKeyword(const TCHAR* prompt, const TCHAR* keyword_buff, TCHAR* result_value);
};

