#pragma once

#ifdef POLA_DLL_PILLAR_TOOLS
#define POLADLL_PILLAR_TOOLS_API __declspec(dllexport)
#else
#define POLADLL_PILLAR_TOOLS_API __declspec(dllimport)
#endif 