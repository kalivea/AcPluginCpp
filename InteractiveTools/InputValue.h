#pragma once
class POLADLL_INTERACTIVE_API InputValue
{
public:
	static bool GetKeyword(const TCHAR* prompt, const TCHAR* keywords, TCHAR* result, size_t buffer_size);
};