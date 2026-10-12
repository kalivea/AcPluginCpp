#include "StdAfx.h"
#include "AddToModelSpace.h"
#include "DbObjectOwner.h"

namespace
{
Acad::ErrorStatus appendEntity(AcDbEntity* entity, AcDbObjectId& entity_id, bool close_entity, bool& transferred)
{
    transferred = false;
    entity_id = AcDbObjectId::kNull;
    if (entity == nullptr || !entity->objectId().isNull()) return Acad::eInvalidInput;
    AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase();
    if (database == nullptr) return Acad::eNoDatabase;

    AcDbBlockTable* table_ptr = nullptr;
    Acad::ErrorStatus status = database->getBlockTable(table_ptr, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTable> table(table_ptr);
    AcDbBlockTableRecord* record_ptr = nullptr;
    status = table->getAt(ACDB_MODEL_SPACE, record_ptr, AcDb::kForWrite);
    if (status != Acad::eOk) return Pola::Close(table, status);
    Pola::OpenedObject<AcDbBlockTableRecord> record(record_ptr);

    AcDbObjectId inserted_id = AcDbObjectId::kNull;
    status = record->appendAcDbEntity(inserted_id, entity);
    if (status == Acad::eOk)
    {
        transferred = true;
        // The database owns entity from this point, even if close fails.
        if (close_entity) status = entity->close();
    }
    status = Pola::Close(record, status);
    status = Pola::Close(table, status);
    if (status == Acad::eOk) entity_id = inserted_id;
    return status;
}
}

Acad::ErrorStatus AddToModelSpace::AddEntityToModelSpace(AcDbEntity* entity, AcDbObjectId& entity_id, bool close_entity)
{
    bool transferred = false;
    return appendEntity(entity, entity_id, close_entity, transferred);
}

AcDbObjectId AddToModelSpace::AddEntityToModelSpace(AcDbEntity* entity)
{
    AcDbObjectId id = AcDbObjectId::kNull;
    // Check residency while the pointer is still valid, before any close.
    const bool was_transient = entity != nullptr && entity->objectId().isNull();
    bool transferred = false;
    const Acad::ErrorStatus status = appendEntity(entity, id, true, transferred);
    if (status != Acad::eOk)
    {
        if (was_transient && !transferred) delete entity;
        acutPrintf(_T("\nAddEntityToModelSpace failed: %d"), static_cast<int>(status));
    }
    return id;
}

AcDbObjectIdArray AddToModelSpace::AddEntityToModelSpace(std::vector<AcDbEntity*> entities)
{
    AcDbObjectIdArray ids;
    // Consume every distinct transient pointer exactly once, including when a
    // previous append failed. Keep positional null IDs for the legacy contract.
    std::vector<AcDbEntity*> consumed;
    for (AcDbEntity* entity : entities)
    {
        bool duplicate = false;
        for (AcDbEntity* previous : consumed) if (previous == entity) duplicate = true;
        if (duplicate)
        {
            ids.append(AcDbObjectId::kNull);
            acutPrintf(_T("\nAddEntityToModelSpace failed: duplicate entity pointer"));
            continue;
        }
        consumed.push_back(entity);
        ids.append(AddEntityToModelSpace(entity));
    }
    return ids;
}
