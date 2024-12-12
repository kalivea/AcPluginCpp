#include "stdafx.h"
#include "StyleTools.h"

AcDbObjectId StyleTools::InitTextStyle()
{
	AcDbTextStyleTable* text_style_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(text_style_table, OpenMode::kForWrite);
	AcDbTextStyleTableRecord* text_style_table_record = new AcDbTextStyleTableRecord();
	text_style_table_record->setName(_T("pt_txt"));
	text_style_table_record->setFileName(_T("tssdeng.shx"));
	text_style_table_record->setXScale(0.7);
	text_style_table->add(text_style_table_record);

	AcDbObjectId default_text_style_id = AcDbObjectId::kNull;
	default_text_style_id = text_style_table_record->objectId();

	text_style_table_record->close();
	text_style_table->close();

	return default_text_style_id;
}

AcDbObjectId StyleTools::InitDimStyle()						//TODO
{
	AcDbObjectId default_dim_style_id = AcDbObjectId::kNull;
	AcDbDimStyleTable* dim_style_table;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(dim_style_table, AcDb::kForWrite);
	AcDbDimStyleTableRecord* dim_style_table_record;
	dim_style_table_record = new AcDbDimStyleTableRecord();
	
	dim_style_table_record->setName(_T("pt_100_350"));
	dim_style_table_record->setDimtxsty(StyleTools::GetTextStyleId(_T("dim_text")));
	dim_style_table_record->setDimtxt(350);
	dim_style_table_record->setDimblk(_T("_ARCHTICK"));
	dim_style_table_record->setDimdle(0);
	dim_style_table_record->setDimdli(0);
	dim_style_table_record->setDimexe(100);
	dim_style_table_record->setDimexo(150);
	dim_style_table_record->setDimfxlenOn(false);
	dim_style_table_record->setDimasz(150);
	dim_style_table_record->setDimtad(2);
	dim_style_table_record->setDimlfac(1);
	dim_style_table_record->setDimdec(0);
	dim_style_table_record->setDimtih(0);		


	dim_style_table->add(dim_style_table_record);
	default_dim_style_id = dim_style_table_record->objectId();
	
	dim_style_table_record->close();
	dim_style_table->close();
	return default_dim_style_id;
}


AcDbObjectId StyleTools::CreateTextStyle(const TCHAR* text_style_name, const TCHAR* big_font_file_name, const TCHAR* text_file_name, const double& x_scale)
{
	AcDbObjectId default_text_style_id = AcDbObjectId::kNull;
	AcDbTextStyleTable* text_style_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(text_style_table, OpenMode::kForWrite);
	AcDbTextStyleTableRecord* text_style_table_record = new AcDbTextStyleTableRecord();
	text_style_table_record->setName(text_style_name);
	text_style_table_record->setBigFontFileName(big_font_file_name);
	text_style_table_record->setFileName(text_file_name);
	text_style_table_record->setXScale(x_scale);
	text_style_table->add(text_style_table_record);

	default_text_style_id = text_style_table_record->objectId();

	text_style_table_record->close();
	text_style_table->close();

	return default_text_style_id;
}

AcDbObjectId StyleTools::GetTextStyleId(const TCHAR* text_style_name)
{
	AcDbObjectId text_style_id = AcDbObjectId::kNull;
	AcDbTextStyleTable* text_style_table = nullptr;
	if (IsTextStyleExist(text_style_name))
	{
		acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(text_style_table, OpenMode::kForRead);
		text_style_table->getAt(text_style_name, text_style_id);
		text_style_table->close();
		return text_style_id;
	}
	else
	{
		text_style_table->close();
		return text_style_id;
	}
}

AcDbObjectId StyleTools::CreateDimensionStyle(const TCHAR* dimension_style_name)
{
	return AcDbObjectId();
}

AcDbObjectId StyleTools::GetDimensionStyleId(const TCHAR* dimension_style_name)
{
	AcDbObjectId dimension_style_id = AcDbObjectId::kNull;
	AcDbDimStyleTable* dimension_style_table = nullptr;
	if (IsDimensionStyleExist(dimension_style_name))
	{
		acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(dimension_style_table, OpenMode::kForRead);
		dimension_style_table->getAt(dimension_style_name, dimension_style_id);
		dimension_style_table->close();
		return dimension_style_id;
	}
	else
	{
		dimension_style_table->close();
		return dimension_style_id;
	}
}

AcDbObjectId StyleTools::CreateLayerStyle(const TCHAR* layer_name)
{
	AcDbObjectId layer_id = AcDbObjectId::kNull;
	AcDbLayerTable* layer_table = nullptr;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(layer_table, OpenMode::kForWrite);
	AcDbLayerTableRecord* layer_table_record = new AcDbLayerTableRecord();
	layer_table_record->setName(layer_name);								//TODO: only set layer name.
	layer_table->add(layer_table_record);
	layer_id = layer_table_record->objectId();

	layer_table_record->close();
	layer_table->close();

	return layer_id;
}

AcDbObjectId StyleTools::GetLayerId(const TCHAR* layer_name)
{

	AcDbObjectId layer_id = AcDbObjectId::kNull;
	AcDbLayerTable* layer_table = nullptr;
	if (IsLayerExist(layer_name))
	{
		acdbHostApplicationServices()->workingDatabase()->getLayerTable(layer_table, OpenMode::kForRead);
		layer_table->getAt(layer_name, layer_id);
		layer_table->close();
		return layer_id;
	}
	else
	{
		layer_table->close();
		return layer_id;
	}
}

bool StyleTools::IsLayerExist(const TCHAR* layer_name)
{
	AcDbLayerTable* layer_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(layer_table, OpenMode::kForRead);
	if (layer_table->has(layer_name))
	{
		layer_table->close();
		return true;
	}
	else
	{
		layer_table->close();
		return false;
	}
}

bool StyleTools::IsDimensionStyleExist(const TCHAR* dimension_style)
{
	AcDbDimStyleTable* dimension_table = nullptr;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(dimension_table, OpenMode::kForRead);
	if (dimension_table->has(dimension_style))
	{
		dimension_table->close();
		return true;
	}
	else
	{
		dimension_table->close();
		return false;
	}
}

bool StyleTools::IsTextStyleExist(const TCHAR* text_style_name)
{
	AcDbTextStyleTable* text_style_table = nullptr;

	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(text_style_table, OpenMode::kForRead);
	if (text_style_table->has(text_style_name))
	{
		text_style_table->close();
		return true;
	}
	else
	{
		text_style_table->close();
		return false;
	}
}
