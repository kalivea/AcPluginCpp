#pragma once
#ifdef POLA_DLL_INTERACTIVE
#define POLADLL_INTERACTIVE_API __declspec(dllexport)
#else
#define POLADLL_INTERACTIVE_API __declspec(dllimport)
#endif 