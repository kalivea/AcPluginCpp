#include "StdAfx.h"
#include "EditDwg.h"
#include "EditBlock.h"
#include "DbObjectOwner.h"

Acad::ErrorStatus EditDwg::CopyBlockDefFromOtherDwg(const TCHAR* file, const TCHAR* name, AcDbObjectId& id)
{
    id = AcDbObjectId::kNull;
    if (file == nullptr || *file == 0 || name == nullptr || *name == 0) return Acad::eInvalidInput;
    AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase();
    if (database == nullptr) return Acad::eNoDatabase;
    std::unique_ptr<AcDbDatabase> source(new AcDbDatabase(false));
    Acad::ErrorStatus status = source->readDwgFile(file, AcDbDatabase::OpenMode::kForReadAndAllShare);
    if (status == Acad::eOk) status = source->closeInput(true);
    if (status != Acad::eOk) return status;
    AcDbObjectId source_id;
    status = EditBlock::GetBlockId(name, source_id, source.get());
    if (status != Acad::eOk) return status;
    AcDbDatabase* temporary_ptr = nullptr;
    status = source->wblock(temporary_ptr, source_id);
    std::unique_ptr<AcDbDatabase> temporary(temporary_ptr);
    if (status != Acad::eOk) return status;
    if (!temporary) return Acad::eNoDatabase;
    AcDbObjectId inserted;
    status = database->insert(inserted, name, temporary.get());
    if (status == Acad::eOk) id = inserted;
    return status;
}
AcDbObjectId EditDwg::CopyBlockDefFromOtherDwg(const TCHAR* file, const TCHAR* name)
{
    AcDbObjectId id;
    const Acad::ErrorStatus status = CopyBlockDefFromOtherDwg(file, name, id);
    if (status != Acad::eOk) acutPrintf(_T("\nCopyBlockDefFromOtherDwg failed: %d"), static_cast<int>(status));
    return id;
}
Acad::ErrorStatus EditDwg::InsertDwgAsBlockDef(const TCHAR* file, const TCHAR* name, bool overwrite, AcDbObjectId& id, AcDbDatabase* database)
{
    id = AcDbObjectId::kNull;
    if (file == nullptr || *file == 0 || name == nullptr || *name == 0 || database == nullptr) return Acad::eInvalidInput;
    AcDbObjectId existing;
    Acad::ErrorStatus status = EditBlock::GetBlockId(name, existing, database);
    if (status != Acad::eOk && status != Acad::eKeyNotFound) return status;
    if (!existing.isNull() && !overwrite) { id = existing; return Acad::eOk; }

    // Finish reading the replacement before changing the destination. A bad
    // path or invalid DWG must never erase the previous block definition.
    std::unique_ptr<AcDbDatabase> source(new AcDbDatabase(false));
    status = source->readDwgFile(file, AcDbDatabase::OpenMode::kForReadAndAllShare);
    if (status == Acad::eOk) status = source->closeInput(true);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbBlockTableRecord> previous;
    if (!existing.isNull())
    {
        AcDbBlockTableRecord* pointer = nullptr;
        status = acdbOpenObject(pointer, existing, AcDb::kForWrite);
        if (status != Acad::eOk) return status;
        previous.reset(pointer);
        status = previous->erase();
        if (status != Acad::eOk) return Pola::Close(previous, status);
    }
    AcDbObjectId inserted;
    status = database->insert(inserted, name, source.get());
    if (status == Acad::eOk) status = EditBlock::CenterBlockInsertPoint(inserted);
    if (status != Acad::eOk)
    {
        if (!inserted.isNull())
        {
            AcDbBlockTableRecord* inserted_ptr = nullptr;
            const Acad::ErrorStatus opened = acdbOpenObject(inserted_ptr, inserted, AcDb::kForWrite);
            if (opened == Acad::eOk)
            {
                Pola::OpenedObject<AcDbBlockTableRecord> replacement(inserted_ptr);
                const Acad::ErrorStatus cleanup = Pola::Close(replacement, replacement->erase());
                if (cleanup != Acad::eOk) acutPrintf(_T("\nReplacement block cleanup failed: %d"), static_cast<int>(cleanup));
            }
            else acutPrintf(_T("\nReplacement block cleanup failed: %d"), static_cast<int>(opened));
        }
        if (previous)
        {
            const Acad::ErrorStatus restore = previous->erase(false);
            if (restore != Acad::eOk) acutPrintf(_T("\nPrevious block restore failed: %d"), static_cast<int>(restore));
        }
    }
    status = Pola::Close(previous, status);
    if (status == Acad::eOk) id = inserted;
    return status;
}
AcDbObjectId EditDwg::InsertDwgAsBlockDef(const TCHAR* file, const TCHAR* name, bool overwrite, AcDbDatabase* database)
{
    AcDbObjectId id;
    const Acad::ErrorStatus status = InsertDwgAsBlockDef(file, name, overwrite, id, database);
    if (status != Acad::eOk) acutPrintf(_T("\nInsertDwgAsBlockDef failed: %d"), static_cast<int>(status));
    return id;
}
