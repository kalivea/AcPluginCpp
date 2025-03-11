#pragma once
#ifdef POLA_DLL_IRM
#define POLADLL_IRM_API __declspec(dllexport)
#else
#define POLADLL_IRM_API __declspec(dllimport)
#endif 