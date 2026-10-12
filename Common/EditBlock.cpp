#include "StdAfx.h"
#include "EditBlock.h"
#include "DbObjectOwner.h"
#include <cmath>

Acad::ErrorStatus EditBlock::IsBlockExist(const TCHAR* name, bool& exists, AcDbDatabase* database)
{
    exists = false;
    if (name == nullptr || *name == 0 || database == nullptr) return Acad::eInvalidInput;
    AcDbBlockTable* pointer = nullptr;
    Acad::ErrorStatus status = database->getBlockTable(pointer, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTable> table(pointer);
    const bool found = table->has(name);
    status = Pola::Close(table);
    if (status == Acad::eOk) exists = found;
    return status;
}
bool EditBlock::IsBlockExist(const TCHAR* name, AcDbDatabase* database)
{
    bool exists = false;
    const Acad::ErrorStatus status = IsBlockExist(name, exists, database);
    if (status != Acad::eOk) acutPrintf(_T("\nIsBlockExist failed: %d"), static_cast<int>(status));
    return exists;
}
Acad::ErrorStatus EditBlock::GetBlockId(const TCHAR* name, AcDbObjectId& id, AcDbDatabase* database)
{
    id = AcDbObjectId::kNull;
    if (name == nullptr || *name == 0 || database == nullptr) return Acad::eInvalidInput;
    AcDbBlockTable* pointer = nullptr;
    Acad::ErrorStatus status = database->getBlockTable(pointer, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTable> table(pointer);
    AcDbObjectId found;
    status = table->getAt(name, found);
    status = Pola::Close(table, status);
    if (status == Acad::eOk) id = found;
    return status;
}
AcDbObjectId EditBlock::GetBlockId(const TCHAR* name, AcDbDatabase* database)
{
    AcDbObjectId id;
    const Acad::ErrorStatus status = GetBlockId(name, id, database);
    if (status != Acad::eOk && status != Acad::eKeyNotFound) acutPrintf(_T("\nGetBlockId failed: %d"), static_cast<int>(status));
    return id;
}
Acad::ErrorStatus EditBlock::CenterBlockInsertPoint(AcDbObjectId id)
{
    if (id.isNull()) return Acad::eInvalidInput;
    AcDbBlockTableRecord* pointer = nullptr;
    Acad::ErrorStatus status = acdbOpenObject(pointer, id, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTableRecord> definition(pointer);
    AcDbBlockTableRecordIterator* iterator_ptr = nullptr;
    status = definition->newIterator(iterator_ptr);
    if (status != Acad::eOk) return Pola::Close(definition, status);
    std::unique_ptr<AcDbBlockTableRecordIterator> iterator(iterator_ptr);
    std::vector<Pola::OpenedObject<AcDbEntity>> entities;
    AcDbExtents extents;
    bool has_extents = false;
    for (iterator->start(); !iterator->done(); iterator->step())
    {
        AcDbEntity* entity_ptr = nullptr;
        status = iterator->getEntity(entity_ptr, AcDb::kForWrite);
        if (status != Acad::eOk) break;
        entities.emplace_back(entity_ptr);
        AcDbExtents current;
        status = entity_ptr->getGeomExtents(current);
        if (status != Acad::eOk) break;
        if (!has_extents) { extents = current; has_extents = true; }
        else extents.addExt(current);
    }
    if (status == Acad::eOk && !has_extents) status = Acad::eInvalidExtents;
    if (status == Acad::eOk)
    {
        const AcGePoint3d center = BasicTools::GetMidPoint(extents);
        if (!std::isfinite(center.x) || !std::isfinite(center.y) || !std::isfinite(center.z)) status = Acad::eInvalidExtents;
        else
        {
            AcGeMatrix3d transform;
            transform.setToTranslation(-center.asVector());
            size_t transformed = 0;
            for (; transformed < entities.size(); ++transformed)
            {
                status = entities[transformed]->transformBy(transform);
                if (status != Acad::eOk) break;
            }
            if (status != Acad::eOk)
            {
                AcGeMatrix3d restore;
                restore.setToTranslation(center.asVector());
                while (transformed > 0)
                {
                    const Acad::ErrorStatus cleanup = entities[--transformed]->transformBy(restore);
                    if (cleanup != Acad::eOk) acutPrintf(_T("\nBlock centering rollback failed: %d"), static_cast<int>(cleanup));
                }
            }
        }
    }
    for (auto& entity : entities) status = Pola::Close(entity, status);
    iterator.reset();
    return Pola::Close(definition, status);
}
bool EditBlock::SetBlockInsertPointToCenter(AcDbObjectId id)
{
    const Acad::ErrorStatus status = CenterBlockInsertPoint(id);
    if (status != Acad::eOk) acutPrintf(_T("\nSetBlockInsertPointToCenter failed: %d"), static_cast<int>(status));
    return status == Acad::eOk;
}
Acad::ErrorStatus EditBlock::InsertBlockRef(AcDbObjectId definition_id, const AcGePoint3d& point, double scale, double rotation, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (definition_id.isNull() || !std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z) || !std::isfinite(rotation)) return Acad::eInvalidInput;
    AcDbBlockTableRecord* definition_ptr = nullptr;
    Acad::ErrorStatus status = acdbOpenObject(definition_ptr, definition_id, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTableRecord> definition(definition_ptr);
    status = Pola::Close(definition);
    if (status != Acad::eOk) return status;
    Pola::DbObjectOwner<AcDbBlockReference> reference(new AcDbBlockReference(point, definition_id));
    status = SetScale(reference.get(), scale);
    if (status == Acad::eOk) status = reference->setRotation(rotation);
    if (status != Acad::eOk) return status;
    status = AddToModelSpace::AddEntityToModelSpace(reference.get(), id, false);
    if (!reference->objectId().isNull())
    {
        const Acad::ErrorStatus cleanup = reference.release()->close();
        if (status == Acad::eOk) status = cleanup;
    }
    if (status != Acad::eOk) id = AcDbObjectId::kNull;
    return status;
}
AcDbObjectId EditBlock::InsertBlockRef(AcDbObjectId definition_id, const AcGePoint3d& point, double scale, double rotation)
{
    AcDbObjectId id;
    const Acad::ErrorStatus status = InsertBlockRef(definition_id, point, scale, rotation, id);
    if (status != Acad::eOk) acutPrintf(_T("\nInsertBlockRef failed: %d"), static_cast<int>(status));
    return id;
}
Acad::ErrorStatus EditBlock::SetScale(AcDbBlockReference* reference, double scale)
{
    if (reference == nullptr || !std::isfinite(scale) || scale <= 0) return Acad::eInvalidInput;
    return reference->setScaleFactors(AcGeScale3d(scale, scale, scale));
}
