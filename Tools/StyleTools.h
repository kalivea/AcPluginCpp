#pragma once
#include "StdAfx.h"

class POLADLL_TOOLS_API StyleTools
{
public:
	static AcDbObjectId InitTextStyle();
	static AcDbObjectId InitDimStyle();
	static AcDbObjectId InitMLeaderStyle();

	static AcDbObjectId CreateTextStyle(const TCHAR* text_style_name, const TCHAR* big_font_file_name, const TCHAR* text_file_name, const double& x_scale = 0.7);
	static AcDbObjectId GetTextStyleId(const TCHAR* text_style_name);

	static AcDbObjectId CreateDimensionStyle(const TCHAR* dimension_style_name);
	static AcDbObjectId GetDimensionStyleId(const TCHAR* dimension_style_name);

	static AcDbObjectId CreateLayerStyle(const TCHAR* layer_name, const int color_index, const TCHAR* line_type);
	static AcDbObjectId GetLayerId(const TCHAR* layer_name);

	static AcDbObjectId LoadLineType(const TCHAR* line_type, const TCHAR* line_type_file);
	static AcDbObjectId GetLineStyleId(const TCHAR* line_type);

	static bool IsLayerExist(const TCHAR* layer_name);
	static bool IsDimensionStyleExist(const TCHAR* dimension_style);
	static bool IsTextStyleExist(const TCHAR* text_style_name);
	static bool IsLineTypeExist(const TCHAR* line_type);
};
