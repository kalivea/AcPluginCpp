#pragma once
#ifdef POLA_DLL_GRID
#define POLADLL_GRID_API __declspec(dllexport)
#else
#define POLADLL_GRID_API __declspec(dllimport)
#endif 
