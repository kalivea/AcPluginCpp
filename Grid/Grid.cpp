#include "StdAfx.h"
#include "Grid.h"
#include "../Common/DbObjectOwner.h"
#include <cmath>

namespace
{
template<class T>
Acad::ErrorStatus CloseAxisOwner(Pola::DbObjectOwner<T>& owner, Acad::ErrorStatus status = Acad::eOk) {
    if (!owner) return status;
    if (owner->objectId().isNull()) { owner.reset(); return status; }
    const Acad::ErrorStatus cleanup = owner.release()->close();
    return status == Acad::eOk ? cleanup : status;
}
void ReportAxisError(Acad::ErrorStatus status) {
    if (status != Acad::eOk) acutPrintf(_T("\nAxis operation failed: %d\n"), static_cast<int>(status));
}
void EraseAxis(AcDbEntity* entity) {
    if (entity && !entity->objectId().isNull()) ReportAxisError(entity->erase());
}

Acad::ErrorStatus CreateAxis(const TCHAR* name, bool vertical, double radius, double length, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!std::isfinite(radius) || !std::isfinite(length) || radius <= 0 || length <= 0 || !std::isfinite(radius + length)) return Acad::eInvalidInput;
    AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
    if (!db) return Acad::eNoDatabase;
    AcDbBlockTable* raw_table = nullptr;
    Acad::ErrorStatus status = db->getBlockTable(raw_table, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTable> table(raw_table);
    if (table->has(name))
    {
        AcDbObjectId existing;
        status = table->getAt(name, existing);
        status = Pola::Close(table, status);
        if (status == Acad::eOk) id = existing;
        return status;
    }
    AcDbObjectId text_style;
    status = StyleTools::GetTextStyleId(_T("grid_text"), text_style);
    if (status != Acad::eOk) return Pola::Close(table, status);
    Pola::DbObjectOwner<AcDbBlockTableRecord> record(new AcDbBlockTableRecord());
    status = record->setName(name);
    AcDbObjectId result;
    if (status == Acad::eOk) status = table->add(result, record.get());
    if (status != Acad::eOk) return Pola::Close(table, status);
    AcGePoint3d start = vertical ? AcGePoint3d(0, radius, 0) : AcGePoint3d(radius, 0, 0);
    AcGePoint3d end = vertical ? AcGePoint3d(0, radius + length, 0) : AcGePoint3d(radius + length, 0, 0);
    Pola::DbObjectOwner<AcDbCircle> circle(new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, radius));
    Pola::DbObjectOwner<AcDbLine> line(new AcDbLine(start, end));
    Pola::DbObjectOwner<AcDbAttributeDefinition> attribute(new AcDbAttributeDefinition(AcGePoint3d::kOrigin, _T("index_num"), _T("ID"), _T("Input axis id"), text_style));
    status = attribute->setHeight(750);
    if (status == Acad::eOk) status = attribute->setHorizontalMode(AcDb::kTextMid);
    AcDbEntity* entities[] = { circle.get(), line.get(), attribute.get() };
    for (AcDbEntity* entity : entities)
    {
        if (status != Acad::eOk) break;
        status = record->appendAcDbEntity(entity);
    }
    // Close successful children before their containing record.
    status = CloseAxisOwner(circle, status);
    status = CloseAxisOwner(line, status);
    status = CloseAxisOwner(attribute, status);
    if (status != Acad::eOk) ReportAxisError(record->erase());
    status = CloseAxisOwner(record, status);
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = result;
    return status;
}
Acad::ErrorStatus InsertAxis(const TCHAR* name, const AcGePoint3d& point, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (!std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z)) return Acad::eInvalidInput;
    AcDbObjectId block_id;
    Acad::ErrorStatus status = EditBlock::GetBlockId(name, block_id);
    if (status != Acad::eOk) return status;
    AcDbBlockTableRecord* raw = nullptr;
    status = acdbOpenObject(raw, block_id, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTableRecord> record(raw);
    AcDbBlockTableRecordIterator* raw_iterator = nullptr;
    status = record->newIterator(raw_iterator);
    if (status != Acad::eOk) return Pola::Close(record, status);
    std::unique_ptr<AcDbBlockTableRecordIterator> iterator(raw_iterator);
    Pola::DbObjectOwner<AcDbBlockReference> reference(new AcDbBlockReference(point, block_id));
    // Attributes become database objects only after their owner is resident.
    status = AddToModelSpace::AddEntityToModelSpace(reference.get(), id, false);
    if (status != Acad::eOk) {
        EraseAxis(reference.get());
        iterator.reset();
        status = Pola::Close(record, status);
        return CloseAxisOwner(reference, status);
    }
    for (; !iterator->done(); iterator->step())
    {
        AcDbEntity* raw_entity = nullptr;
        status = iterator->getEntity(raw_entity, AcDb::kForRead);
        if (status != Acad::eOk) break;
        Pola::OpenedObject<AcDbEntity> entity(raw_entity);
        const AcDbAttributeDefinition* definition = AcDbAttributeDefinition::cast(entity.get());
        if (definition && !definition->isConstant()) status = Grid::AddAttribute(reference.get(), definition);
        status = Pola::Close(entity, status);
        if (status != Acad::eOk) break;
    }
    iterator.reset();
    status = Pola::Close(record, status);
    if (status != Acad::eOk) EraseAxis(reference.get());
    status = CloseAxisOwner(reference, status);
    if (status != Acad::eOk) id = AcDbObjectId::kNull;
    return status;
}
}
Acad::ErrorStatus Grid::CreateVerticalAxis(AcDbObjectId& id) { return CreateAxis(_T("grid_v"), true, circle_radius_, line_length_, id); }
Acad::ErrorStatus Grid::CreateHorizonAxis(AcDbObjectId& id) { return CreateAxis(_T("grid_h"), false, circle_radius_, line_length_, id); }
AcDbObjectId Grid::CreateVerticalAxis() { AcDbObjectId id; ReportAxisError(CreateVerticalAxis(id)); return id; }
AcDbObjectId Grid::CreateHorizonAxis() { AcDbObjectId id; ReportAxisError(CreateHorizonAxis(id)); return id; }
Acad::ErrorStatus Grid::InsertVerticalAxis(const AcGePoint3d& p, AcDbObjectId& id) { return InsertAxis(_T("grid_v"), p, id); }
Acad::ErrorStatus Grid::InsertHorizonAxis(const AcGePoint3d& p, AcDbObjectId& id) { return InsertAxis(_T("grid_h"), p, id); }
AcDbObjectId Grid::InsertVerticalAxis(const AcGePoint3d& p) { AcDbObjectId id; ReportAxisError(InsertVerticalAxis(p, id)); return id; }
AcDbObjectId Grid::InsertHorizonAxis(const AcGePoint3d& p) { AcDbObjectId id; ReportAxisError(InsertHorizonAxis(p, id)); return id; }

Acad::ErrorStatus Grid::AddAttribute(AcDbBlockReference* reference, const AcDbAttributeDefinition* definition)
{
    if (!reference || !definition || reference->objectId().isNull()) return Acad::eInvalidInput;
    Pola::DbObjectOwner<AcDbAttribute> attribute(new AcDbAttribute());
    Acad::ErrorStatus status = attribute->setAttributeFromBlock(definition, reference->blockTransform());
    if (status != Acad::eOk) return status;
    AcDbObjectId id;
    status = reference->appendAttribute(id, attribute.get());
    return CloseAxisOwner(attribute, status);
}
Acad::ErrorStatus Grid::SetAttribute(AcDbBlockReference* reference, const TCHAR* tag, const TCHAR* value)
{
    if (!reference || !tag || !value) return Acad::eInvalidInput;
    std::unique_ptr<AcDbObjectIterator> iterator(reference->attributeIterator());
    if (!iterator) return Acad::eInvalidInput;
    for (; !iterator->done(); iterator->step())
    {
        AcDbAttribute* raw = nullptr;
        Acad::ErrorStatus status = acdbOpenObject(raw, iterator->objectId(), AcDb::kForWrite);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbAttribute> attribute(raw);
        const TCHAR* stored_tag = attribute->tagConst();
        const bool matches = stored_tag && _tcsicmp(stored_tag, tag) == 0;
        if (matches) status = attribute->setTextString(value);
        status = Pola::Close(attribute, status);
        if (status != Acad::eOk || matches) return status;
    }
    return Acad::eKeyNotFound;
}
Acad::ErrorStatus Grid::DrawVerticalAxis(const AcGePoint3dArray& points, const TCHAR* values[])
{
    if (!values) return Acad::eInvalidInput;
    for (int i = 0; i < points.length(); ++i)
        if (!values[i] || !std::isfinite(points[i].x) || !std::isfinite(points[i].y) || !std::isfinite(points[i].z))
            return Acad::eInvalidInput;
    for (int i = 0; i < points.length(); ++i)
    {
        if (!values[i]) return Acad::eInvalidInput;
        AcDbObjectId id;
        Acad::ErrorStatus status = InsertVerticalAxis(points[i], id);
        if (status != Acad::eOk) return status;
        AcDbBlockReference* raw = nullptr;
        status = acdbOpenObject(raw, id, AcDb::kForWrite);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbBlockReference> reference(raw);
        status = SetAttribute(reference.get(), _T("ID"), values[i]);
        if (status != Acad::eOk) EraseAxis(reference.get());
        status = Pola::Close(reference, status);
        if (status != Acad::eOk) return status;
    }
    return Acad::eOk;
}
Acad::ErrorStatus Grid::DrawHorizonAxis(const AcGePoint3dArray& points, const TCHAR* values[])
{
    if (!values) return Acad::eInvalidInput;
    for (int i = 0; i < points.length(); ++i)
        if (!values[i] || !std::isfinite(points[i].x) || !std::isfinite(points[i].y) || !std::isfinite(points[i].z))
            return Acad::eInvalidInput;
    for (int i = 0; i < points.length(); ++i)
    {
        if (!values[i]) return Acad::eInvalidInput;
        AcDbObjectId id;
        Acad::ErrorStatus status = InsertHorizonAxis(points[i], id);
        if (status != Acad::eOk) return status;
        AcDbBlockReference* raw = nullptr;
        status = acdbOpenObject(raw, id, AcDb::kForWrite);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbBlockReference> reference(raw);
        status = SetAttribute(reference.get(), _T("ID"), values[i]);
        if (status != Acad::eOk) EraseAxis(reference.get());
        status = Pola::Close(reference, status);
        if (status != Acad::eOk) return status;
    }
    return Acad::eOk;
}
