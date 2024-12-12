#pragma once
#include "def.h"
#define POLA_DLL

class POLADLL StyleTools
{
public:
	static AcDbObjectId InitTextStyle();
	static AcDbObjectId InitDimStyle();
	static AcDbObjectId InitLayer();

	static AcDbObjectId CreateTextStyle(const TCHAR* text_style_name, const TCHAR* big_font_file_name, const TCHAR* text_file_name, const double& x_scale = 0.7);
	static AcDbObjectId GetTextStyleId(const TCHAR* text_style_name);

	static AcDbObjectId CreateDimensionStyle(const TCHAR* dimension_style_name);
	static AcDbObjectId GetDimensionStyleId(const TCHAR* dimension_style_name);

	static AcDbObjectId CreateLayerStyle(const TCHAR* layer_name);
	static AcDbObjectId GetLayerId(const TCHAR* layer_name);

	static bool IsLayerExist(const TCHAR* layer_name);
	static bool IsDimensionStyleExist(const TCHAR* dimension_style);
	static bool IsTextStyleExist(const TCHAR* text_style_name);
};

