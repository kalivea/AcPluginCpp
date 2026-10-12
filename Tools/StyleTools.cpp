#include "StdAfx.h"
#include "StyleTools.h"
#include "../Common/DbObjectOwner.h"
#include <cmath>

namespace
{
void ReportStyleError(Acad::ErrorStatus status) {
    if (status != Acad::eOk) acutPrintf(_T("\nStyle operation failed: %d\n"), static_cast<int>(status));
}

template<class Table, class Getter>
Acad::ErrorStatus FindStyle(const TCHAR* name, AcDbObjectId& id, Getter getter)
{
    id = AcDbObjectId::kNull;
    if (!name || !*name) return Acad::eInvalidInput;
    AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase();
    if (!database) return Acad::eNoDatabase;
    Table* raw = nullptr;
    Acad::ErrorStatus status = getter(database, raw);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<Table> table(raw);
    AcDbObjectId result;
    status = table->getAt(name, result);
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = result;
    return status;
}
}

Acad::ErrorStatus StyleTools::GetTextStyleId(const TCHAR* name, AcDbObjectId& id)
{
    return FindStyle<AcDbTextStyleTable>(name, id, [](AcDbDatabase* db, AcDbTextStyleTable*& t) { return db->getTextStyleTable(t, AcDb::kForRead); });
}
Acad::ErrorStatus StyleTools::GetDimensionStyleId(const TCHAR* name, AcDbObjectId& id)
{
    return FindStyle<AcDbDimStyleTable>(name, id, [](AcDbDatabase* db, AcDbDimStyleTable*& t) { return db->getDimStyleTable(t, AcDb::kForRead); });
}
Acad::ErrorStatus StyleTools::GetLayerId(const TCHAR* name, AcDbObjectId& id)
{
    return FindStyle<AcDbLayerTable>(name, id, [](AcDbDatabase* db, AcDbLayerTable*& t) { return db->getLayerTable(t, AcDb::kForRead); });
}
Acad::ErrorStatus StyleTools::GetLineStyleId(const TCHAR* name, AcDbObjectId& id)
{
    return FindStyle<AcDbLinetypeTable>(name, id, [](AcDbDatabase* db, AcDbLinetypeTable*& t) { return db->getLinetypeTable(t, AcDb::kForRead); });
}
Acad::ErrorStatus StyleTools::GetMLeaderStyleId(const TCHAR* name, AcDbObjectId& id)
{
    return FindStyle<AcDbDictionary>(name, id, [](AcDbDatabase* db, AcDbDictionary*& t) { return db->getMLeaderStyleDictionary(t, AcDb::kForRead); });
}

#define POLA_STYLE_WRAPPER(Name, Exists) \
AcDbObjectId StyleTools::Name(const TCHAR* name) { AcDbObjectId id; ReportStyleError(Name(name, id)); return id; } \
bool StyleTools::Exists(const TCHAR* name) { AcDbObjectId id; return Name(name, id) == Acad::eOk; }
POLA_STYLE_WRAPPER(GetTextStyleId, IsTextStyleExist)
POLA_STYLE_WRAPPER(GetDimensionStyleId, IsDimensionStyleExist)
POLA_STYLE_WRAPPER(GetLayerId, IsLayerExist)
POLA_STYLE_WRAPPER(GetLineStyleId, IsLineTypeExist)
POLA_STYLE_WRAPPER(GetMLeaderStyleId, IsMLeaderStyleExist)
#undef POLA_STYLE_WRAPPER

Acad::ErrorStatus StyleTools::InitTextStyle(AcDbObjectId& id) {
    return CreateTextStyle(_T("default_txt"), _T("gbcbig.shx"), _T("gbenor.shx"), 0.75, id);
}
AcDbObjectId StyleTools::InitTextStyle() {
    AcDbObjectId id; ReportStyleError(InitTextStyle(id)); return id;
}
Acad::ErrorStatus StyleTools::CreateTextStyle(const TCHAR* name, const TCHAR* big_font, const TCHAR* font, const double& scale, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!name || !*name || !big_font || !font || !std::isfinite(scale) || scale <= 0) return Acad::eInvalidInput;
    Acad::ErrorStatus status = GetTextStyleId(name, id);
    if (status == Acad::eOk) return status;
    if (status != Acad::eKeyNotFound) return status;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    AcDbTextStyleTable* raw = nullptr;
    status = db->getTextStyleTable(raw, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbTextStyleTable> table(raw);
    Pola::DbObjectOwner<AcDbTextStyleTableRecord> record(new AcDbTextStyleTableRecord());
    status = record->setName(name);
    if (status == Acad::eOk) status = record->setBigFontFileName(big_font);
    if (status == Acad::eOk) status = record->setFileName(font);
    if (status == Acad::eOk) status = record->setXScale(scale);
    AcDbObjectId result;
    if (status == Acad::eOk) status = table->add(result, record.get());
    if (status == Acad::eOk) status = record.release()->close();
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = result;
    return status;
}
AcDbObjectId StyleTools::CreateTextStyle(const TCHAR* name, const TCHAR* big_font, const TCHAR* font, const double& scale)
{
    AcDbObjectId id;
    ReportStyleError(CreateTextStyle(name, big_font, font, scale, id));
    return id;
}

Acad::ErrorStatus StyleTools::InitDimStyle(AcDbObjectId& id) {
    id = AcDbObjectId::kNull;
    Acad::ErrorStatus status = GetDimensionStyleId(_T("pt_100_350"), id);
    if (status != Acad::eKeyNotFound) return status;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    if (!db) return Acad::eNoDatabase;
    AcDbObjectId text_style;
    status = GetTextStyleId(_T("dim_text"), text_style);
    if (status != Acad::eOk) return status;
    AcDbDimStyleTable* raw = nullptr;
    status = db->getDimStyleTable(raw, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbDimStyleTable> table(raw);
    Pola::DbObjectOwner<AcDbDimStyleTableRecord> record(new AcDbDimStyleTableRecord());
    status = record->setName(_T("pt_100_350"));
    if (status == Acad::eOk) status = record->setDimtxsty(text_style);
    if (status == Acad::eOk) status = record->setDimtxt(350);
    if (status == Acad::eOk) status = record->setDimdle(0);
    if (status == Acad::eOk) status = record->setDimdli(0);
    if (status == Acad::eOk) status = record->setDimexe(100);
    if (status == Acad::eOk) status = record->setDimexo(150);
    if (status == Acad::eOk) status = record->setDimfxlenOn(false);
    if (status == Acad::eOk) status = record->setDimasz(150);
    if (status == Acad::eOk) status = record->setDimtad(2);
    if (status == Acad::eOk) status = record->setDimlfac(1);
    if (status == Acad::eOk) status = record->setDimdec(0);
    if (status == Acad::eOk) status = record->setDimtih(0);
    if (status == Acad::eOk) status = record->setDimblk(_T("_ARCHTICK"));
    AcDbObjectId result;
    if (status == Acad::eOk) status = table->add(result, record.get());
    if (status == Acad::eOk) status = record.release()->close();
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = result;
    return status;
}
AcDbObjectId StyleTools::InitDimStyle() {
    AcDbObjectId id; ReportStyleError(InitDimStyle(id)); return id;
}
Acad::ErrorStatus StyleTools::CreateDimensionStyle(const TCHAR* name, AcDbObjectId& id) {
    id = AcDbObjectId::kNull;
    Acad::ErrorStatus status = GetDimensionStyleId(name, id);
    if (status != Acad::eKeyNotFound) return status;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    if (!db) return Acad::eNoDatabase;
    AcDbDimStyleTable* raw = nullptr;
    status = db->getDimStyleTable(raw, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbDimStyleTable> table(raw);
    Pola::DbObjectOwner<AcDbDimStyleTableRecord> record(new AcDbDimStyleTableRecord());
    status = record->setName(name);
    AcDbObjectId result;
    if (status == Acad::eOk) status = table->add(result, record.get());
    if (status == Acad::eOk) status = record.release()->close();
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = result;
    return status;
}
AcDbObjectId StyleTools::CreateDimensionStyle(const TCHAR* name) {
    AcDbObjectId id; ReportStyleError(CreateDimensionStyle(name, id)); return id;
}
Acad::ErrorStatus StyleTools::InitMLeaderStyle(AcDbObjectId& id) {
    id = AcDbObjectId::kNull;
    Acad::ErrorStatus status = GetMLeaderStyleId(_T("NONE_ARROW"), id);
    if (status != Acad::eKeyNotFound) return status;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    if (!db) return Acad::eNoDatabase;
    Pola::DbObjectOwner<AcDbMLeaderStyle> style(new AcDbMLeaderStyle());
    status = style->setName(_T("NONE_ARROW"));
    if (status == Acad::eOk) status = style->setArrowSymbolId(_T("_NONE"));
    AcDbObjectId result;
    if (status == Acad::eOk) status = style->postMLeaderStyleToDb(db, _T("NONE_ARROW"), result);
    if (status == Acad::eOk) status = style.release()->close();
    if (status == Acad::eOk) id = result;
    return status;
}
AcDbObjectId StyleTools::InitMLeaderStyle() {
    AcDbObjectId id; ReportStyleError(InitMLeaderStyle(id)); return id;
}
Acad::ErrorStatus StyleTools::CreateLayerStyle(const TCHAR* name, const int& color_index, const TCHAR* line_type, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!name || !*name || !line_type || color_index < 0 || color_index > 256) return Acad::eInvalidInput;
    Acad::ErrorStatus status = GetLayerId(name, id);
    if (status == Acad::eOk) return status;
    if (status != Acad::eKeyNotFound) return status;
    AcDbObjectId line_id;
    status = GetLineStyleId(line_type, line_id);
    if (status != Acad::eOk) return status;
    AcDbLayerTable* raw = nullptr;
    status = acdbHostApplicationServices()->workingDatabase()->getLayerTable(raw, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbLayerTable> table(raw);
    Pola::DbObjectOwner<AcDbLayerTableRecord> record(new AcDbLayerTableRecord());
    AcCmColor color; color.setColorIndex(static_cast<Adesk::UInt16>(color_index));
    status = record->setName(name);
    if (status == Acad::eOk) status = record->setLinetypeObjectId(line_id);
    if (status == Acad::eOk) record->setColor(color);
    AcDbObjectId result;
    if (status == Acad::eOk) status = table->add(result, record.get());
    if (status == Acad::eOk) status = record.release()->close();
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = result;
    return status;
}
AcDbObjectId StyleTools::CreateLayerStyle(const TCHAR* name, const int& color, const TCHAR* type)
{
    AcDbObjectId id; ReportStyleError(CreateLayerStyle(name, color, type, id)); return id;
}
Acad::ErrorStatus StyleTools::LoadLineType(const TCHAR* name, const TCHAR* file, AcDbObjectId& id) {
    id = AcDbObjectId::kNull;
    if (!file || !*file) return Acad::eInvalidInput;
    Acad::ErrorStatus status = GetLineStyleId(name, id);
    if (status != Acad::eKeyNotFound) return status;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    if (!db) return Acad::eNoDatabase;
    status = db->loadLineTypeFile(name, file);
    if (status != Acad::eOk) return status;
    return GetLineStyleId(name, id);
}
AcDbObjectId StyleTools::LoadLineType(const TCHAR* name, const TCHAR* file) {
    AcDbObjectId id; ReportStyleError(LoadLineType(name, file, id)); return id;
}
