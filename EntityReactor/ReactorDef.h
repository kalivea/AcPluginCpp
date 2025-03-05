#pragma once
#ifdef POLA_DLL_REACTOR
#define POLADLL_REACTOR_API __declspec(dllexport)
#else
#define POLADLL_REACTOR_API __declspec(dllimport)
#endif 