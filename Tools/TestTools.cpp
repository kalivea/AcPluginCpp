#include "stdafx.h"
#include "TestTools.h"

void TestTools::ShowES(Acad::ErrorStatus es,TCHAR* show)
{
	acutPrintf(_T("%s ES = %s\n"), show, es);
}
