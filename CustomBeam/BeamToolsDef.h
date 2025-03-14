#pragma once

#ifdef POLA_DLL_BEAM_TOOLS
#define POLADLL_BEAM_TOOLS_API __declspec(dllexport)
#else
#define POLADLL_BEAM_TOOLS_API __declspec(dllimport)
#endif 