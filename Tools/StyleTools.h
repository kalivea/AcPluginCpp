#pragma once
#include "dbxHeaders.h"
#include "ToolsDef.h"
#include <tchar.h>

class POLADLL_TOOLS_API StyleTools
{
public:
    static Acad::ErrorStatus GetTextStyleId(const TCHAR* name, AcDbObjectId& id);
    static Acad::ErrorStatus GetDimensionStyleId(const TCHAR* name, AcDbObjectId& id);
    static Acad::ErrorStatus GetLayerId(const TCHAR* name, AcDbObjectId& id);
    static Acad::ErrorStatus GetLineStyleId(const TCHAR* name, AcDbObjectId& id);
    static Acad::ErrorStatus GetMLeaderStyleId(const TCHAR* name, AcDbObjectId& id);
    static Acad::ErrorStatus CreateTextStyle(const TCHAR* name, const TCHAR* big_font, const TCHAR* font, const double& scale, AcDbObjectId& id);
    static Acad::ErrorStatus CreateLayerStyle(const TCHAR* name, const int& color, const TCHAR* type, AcDbObjectId& id);
	static Acad::ErrorStatus InitTextStyle(AcDbObjectId& id);
    static Acad::ErrorStatus InitDimStyle(AcDbObjectId& id);
    static Acad::ErrorStatus InitMLeaderStyle(AcDbObjectId& id);
    static Acad::ErrorStatus CreateDimensionStyle(const TCHAR* name, AcDbObjectId& id);
    static Acad::ErrorStatus LoadLineType(const TCHAR* name, const TCHAR* file, AcDbObjectId& id);
	static AcDbObjectId InitTextStyle();
	static AcDbObjectId InitDimStyle();
	static AcDbObjectId InitMLeaderStyle();

	static AcDbObjectId CreateTextStyle(const TCHAR* text_style_name, const TCHAR* big_font_file_name, const TCHAR* text_file_name, const double& x_scale = 0.7);
	static AcDbObjectId GetTextStyleId(const TCHAR* text_style_name);

	static AcDbObjectId CreateDimensionStyle(const TCHAR* dimension_style_name);
	static AcDbObjectId GetDimensionStyleId(const TCHAR* dimension_style_name);

	static AcDbObjectId CreateLayerStyle(const TCHAR* layer_name, const int& color_index, const TCHAR* line_type);
	static AcDbObjectId GetLayerId(const TCHAR* layer_name);

	static AcDbObjectId LoadLineType(const TCHAR* line_type, const TCHAR* line_type_file);
	static AcDbObjectId GetLineStyleId(const TCHAR* line_type);

	static AcDbObjectId GetMLeaderStyleId(const TCHAR* mleader_style_name);

	static bool IsLayerExist(const TCHAR* layer_name);
	static bool IsDimensionStyleExist(const TCHAR* dimension_style);
	static bool IsTextStyleExist(const TCHAR* text_style_name);
	static bool IsLineTypeExist(const TCHAR* line_type);
	static bool IsMLeaderStyleExist(const TCHAR* mleader_style_name);
};
