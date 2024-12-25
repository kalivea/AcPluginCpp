#pragma once
#ifdef POLA_DLL_TOOLS
#define POLADLL_TOOLS_API __declspec(dllexport)
#else
#define POLADLL_TOOLS_API __declspec(dllimport)
#endif 
