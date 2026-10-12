#pragma once
#include <tchar.h>
#include "InteractiveDef.h"

class POLADLL_INTERACTIVE_API DwgFileInfo
{
public:
	static bool GetDwgFileInfoWithExtension(TCHAR* szFileName, int nFileNameSize, TCHAR* szFilePath, int nFilePathSize);
	static bool GetDwgFileNameWithoutExtension(TCHAR* szFileName, int nFileNameSize);
};
