#pragma once

#ifdef POLA_DLL
#define POLADLL __declspec(dllexport)
#else
#define POLADLL __declspec(dllimport)
#endif // POLA_DLL
