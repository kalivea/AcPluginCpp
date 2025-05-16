#pragma once

#ifdef POLA_DLL_ENCLOSURE
#define POLADLL_ENCLOSURE_API __declspec(dllexport)
#else
#define POLADLL_ENCLOSURE_API __declspec(dllimport)
#endif 