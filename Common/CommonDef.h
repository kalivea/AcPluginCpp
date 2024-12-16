#pragma once
#ifdef POLA_DLL_COMMON
#define POLADLL_COMMON_API __declspec(dllexport)
#else
#define POLADLL_COMMON_API __declspec(dllimport)
#endif 
