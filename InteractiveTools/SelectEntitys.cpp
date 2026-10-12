#include "StdAfx.h"
#include "SelectEntitys.h"
#include "../Common/DbObjectOwner.h"
#include <cmath>
#include <algorithm>

namespace {
void ReportSelectionError(Acad::ErrorStatus status) {
    if (status != Acad::eOk && status != Acad::eUserBreak)
        acutPrintf(_T("\nSelection failed: %d\n"), static_cast<int>(status));
}
}
Acad::ErrorStatus SelectEntitys::PickEntitysChecked(const TCHAR* prompt, resbuf* filter, AcDbObjectIdArray& ids) {
    ids.removeAll();
    ads_name selection = {0, 0};
    if (prompt) acutPrintf(_T("%s"), prompt);
    const int selected = acedSSGet(nullptr, nullptr, nullptr, filter, selection);
    if (selected != RTNORM) return selected == RTCAN || selected == RTNONE ? Acad::eUserBreak : Acad::eInvalidInput;
    AcDbObjectIdArray result;
    Adesk::Int32 length = 0;
    Acad::ErrorStatus status = acedSSLength(selection, &length) == RTNORM && length >= 0 ? Acad::eOk : Acad::eInvalidInput;
    for (Adesk::Int32 i = 0; status == Acad::eOk && i < length; ++i) {
        ads_name name;
        AcDbObjectId id;
        if (acedSSName(selection, i, name) != RTNORM) status = Acad::eInvalidInput;
        else status = acdbGetObjectId(id, name);
        if (status == Acad::eOk) result.append(id);
    }
    const Acad::ErrorStatus cleanup = acedSSFree(selection) == RTNORM ? Acad::eOk : Acad::eInvalidInput;
    if (status == Acad::eOk) status = cleanup;
    if (status == Acad::eOk) ids = std::move(result);
    return status;
}
Acad::ErrorStatus SelectEntitys::PickEntitysChecked(const TCHAR* prompt, const std::vector<AcRxClass*>& classes, AcDbObjectIdArray& ids) {
    ids.removeAll();
    if (classes.empty() || std::any_of(classes.begin(), classes.end(), [](AcRxClass* type) { return !type; }))
        return Acad::eInvalidInput;
    AcDbObjectIdArray candidates, result;
    Acad::ErrorStatus status = PickEntitysChecked(prompt, static_cast<resbuf*>(nullptr), candidates);
    if (status != Acad::eOk) return status;
    for (AcDbObjectId id : candidates) {
        AcDbEntity* raw = nullptr;
        status = acdbOpenAcDbEntity(raw, id, AcDb::kForRead);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbEntity> entity(raw);
        for (AcRxClass* type : classes)
            if (entity->isKindOf(type)) { result.append(id); break; }
        if ((status = Pola::Close(entity)) != Acad::eOk) return status;
    }
    ids = std::move(result);
    return Acad::eOk;
}
bool SelectEntitys::PickEntitys(const TCHAR* prompt, resbuf* filter, AcDbObjectIdArray& ids) {
    const Acad::ErrorStatus status = PickEntitysChecked(prompt, filter, ids);
    ReportSelectionError(status);
    return status == Acad::eOk && !ids.isEmpty();
}
bool SelectEntitys::PickEntitys(const TCHAR* prompt, const std::vector<AcRxClass*>& classes, AcDbObjectIdArray& ids) {
    const Acad::ErrorStatus status = PickEntitysChecked(prompt, classes, ids);
    ReportSelectionError(status);
    return status == Acad::eOk && !ids.isEmpty();
}

bool SelectEntitys::PickEntitys(const TCHAR* prompt, AcRxClass* class_describe, AcDbObjectIdArray& entity_id_array)
{
	std::vector<AcRxClass*> class_describe_vector;
	class_describe_vector.push_back(class_describe);
	return PickEntitys(prompt, class_describe_vector, entity_id_array);
}

bool SelectEntitys::PickLinesOnLayer(const TCHAR* layer_name, AcDbObjectIdArray& entity_id_array)
{
	if (!layer_name || !*layer_name) { entity_id_array.removeAll(); return false; }
	struct resbuf* result_buff = acutBuildList(RTDXF0, _T("LINE"), 8, layer_name, RTNONE);
	if (!result_buff) { entity_id_array.removeAll(); return false; }
	bool result = PickEntitys(_T("Select lines on layer: "), result_buff, entity_id_array);
	acutRelRb(result_buff);
	return result;
}

Acad::ErrorStatus SelectEntitys::PickEntitysInRectangelChecked(const TCHAR* prompt, const AcGePoint2d& first,
    const AcGePoint2d& second, AcDbObjectIdArray& ids) {
    ids.removeAll();
    const AcGePoint3d lower = BasicTools::Point2dToPoint3d(first), upper = BasicTools::Point2dToPoint3d(second);
    if (!std::isfinite(first.x) || !std::isfinite(first.y) || !std::isfinite(second.x) || !std::isfinite(second.y) ||
        !BasicTools::CanDrawRect(lower, upper)) return Acad::eInvalidInput;
    if (prompt) acutPrintf(_T("%s"), prompt);
    AcDbObjectIdArray all, result;
    Acad::ErrorStatus status = BasicTools::GetAllEntityIdsInDatabase(all);
    if (status != Acad::eOk) return status;
    for (AcDbObjectId id : all) {
        AcDbEntity* pointer = nullptr;
        if ((status = acdbOpenObject(pointer, id, AcDb::kForRead)) != Acad::eOk) return status;
        Pola::OpenedObject<AcDbEntity> entity(pointer);
        AcDbExtents extents;
        status = entity->getGeomExtents(extents);
        if (status == Acad::eOk && BasicTools::IsIntersectRectangle(lower, upper, extents.minPoint(), extents.maxPoint()))
            result.append(id);
        if ((status = Pola::Close(entity, status)) != Acad::eOk) return status;
    }
    ids = std::move(result);
    return Acad::eOk;
}
bool SelectEntitys::PickEntitysInRectangel(const TCHAR* prompt, const AcGePoint2d& first,
    const AcGePoint2d& second, AcDbObjectIdArray& ids) {
    const Acad::ErrorStatus status = PickEntitysInRectangelChecked(prompt, first, second, ids);
    ReportSelectionError(status);
    return status == Acad::eOk;
}
bool SelectEntitys::PickPoint(const TCHAR* prompt, AcGePoint3d& point) {
    point = AcGePoint3d::kOrigin;
    AcGePoint3d selected;
    if (acedGetPoint(nullptr, prompt, asDblArray(selected)) != RTNORM) return false;
    const Acad::ErrorStatus status = CoordinateSystem::UcsToWcs(selected, point);
    ReportSelectionError(status);
    return status == Acad::eOk;
}
bool SelectEntitys::PickPoint(const TCHAR* prompt, const AcGePoint3d& base, AcGePoint3d& point) {
    const AcGePoint3d base_copy = base;
    point = AcGePoint3d::kOrigin;
    AcGePoint3d ucs_base, selected;
    Acad::ErrorStatus status = CoordinateSystem::WcsToUcs(base_copy, ucs_base);
    if (status != Acad::eOk) { ReportSelectionError(status); return false; }
    if (acedGetPoint(asDblArray(ucs_base), prompt, asDblArray(selected)) != RTNORM) return false;
    status = CoordinateSystem::UcsToWcs(selected, point);
    ReportSelectionError(status);
    return status == Acad::eOk;
}

Acad::ErrorStatus SelectEntitys::GetAllEntitysByTypeChecked(const AcRxClass* type, AcDbObjectIdArray& ids)
{
    ids.removeAll();
    if (!type) return Acad::eInvalidInput;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    if (!db) return Acad::eNoDatabase;
    AcDbBlockTable* raw_table = nullptr;
    Acad::ErrorStatus status = db->getBlockTable(raw_table, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTable> table(raw_table);
    AcDbBlockTableIterator* raw_table_iterator = nullptr;
    status = table->newIterator(raw_table_iterator);
    if (status != Acad::eOk) return status;
    std::unique_ptr<AcDbBlockTableIterator> tables(raw_table_iterator);
    AcDbObjectIdArray result;
    for (; !tables->done(); tables->step())
    {
        AcDbBlockTableRecord* raw_record = nullptr;
        status = tables->getRecord(raw_record, AcDb::kForRead);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbBlockTableRecord> record(raw_record);
        AcDbBlockTableRecordIterator* raw_iterator = nullptr;
        status = record->newIterator(raw_iterator);
        if (status != Acad::eOk) return status;
        std::unique_ptr<AcDbBlockTableRecordIterator> entities(raw_iterator);
        for (; !entities->done(); entities->step())
        {
            AcDbEntity* raw_entity = nullptr;
            status = entities->getEntity(raw_entity, AcDb::kForRead);
            if (status != Acad::eOk) return status;
            Pola::OpenedObject<AcDbEntity> entity(raw_entity);
            if (entity->isKindOf(type)) result.append(entity->objectId());
            status = Pola::Close(entity);
            if (status != Acad::eOk) return status;
        }
        entities.reset();
        status = Pola::Close(record);
        if (status != Acad::eOk) return status;
    }
    tables.reset();
    status = Pola::Close(table);
    if (status == Acad::eOk) ids = result;
    return status;
}
bool SelectEntitys::GetAllEntitysByType(const AcRxClass* type, AcDbObjectIdArray& ids)
{
    return GetAllEntitysByTypeChecked(type, ids) == Acad::eOk && !ids.isEmpty();
}
