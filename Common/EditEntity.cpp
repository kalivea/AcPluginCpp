#include "StdAfx.h"
#include "EditEntity.h"
#include "DbObjectOwner.h"
#include <cmath>

namespace
{
bool finitePoint(const AcGePoint3d& p) { return std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z); }
template<class Operation> Acad::ErrorStatus edit(const AcDbObjectId& id, Operation operation)
{
    if (id.isNull()) return Acad::eInvalidInput;
    AcDbEntity* pointer = nullptr;
    Acad::ErrorStatus status = acdbOpenObject(pointer, id, AcDb::kForWrite);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbEntity> entity(pointer);
    return Pola::Close(entity, operation(entity.get()));
}
template<class Operation> Acad::ErrorStatus each(const AcDbObjectIdArray& ids, Operation operation)
{
    for (int i = 0; i < ids.length(); ++i)
    {
        const Acad::ErrorStatus status = operation(ids.at(i), i);
        if (status != Acad::eOk) return status;
    }
    return Acad::eOk;
}
Acad::ErrorStatus appendOwned(Pola::DbObjectOwner<AcDbEntity>& entity, AcDbObjectId& id)
{
    Acad::ErrorStatus status = AddToModelSpace::AddEntityToModelSpace(entity.get(), id, false);
    if (!entity->objectId().isNull())
    {
        const Acad::ErrorStatus cleanup = entity.release()->close();
        if (status == Acad::eOk) status = cleanup;
    }
    if (status != Acad::eOk) id = AcDbObjectId::kNull;
    return status;
}
void report(const TCHAR* operation, Acad::ErrorStatus status)
{
    if (status != Acad::eOk) acutPrintf(_T("\n%s failed: %d"), operation, static_cast<int>(status));
}
}

Acad::ErrorStatus EditEntity::SetColor(const AcDbObjectId& id, const unsigned char& index)
{
    return edit(id, [index](AcDbEntity* entity) { return entity->setColorIndex(index); });
}
Acad::ErrorStatus EditEntity::SetColor(const AcDbObjectIdArray& ids, const unsigned char& index)
{
    return each(ids, [index](const AcDbObjectId& id, int) { return SetColor(id, index); });
}
Acad::ErrorStatus EditEntity::SetColor(const AcDbObjectIdArray& ids, const unsigned char indices[])
{
    if (indices == nullptr) return Acad::eInvalidInput;
    return each(ids, [indices](const AcDbObjectId& id, int i) { return SetColor(id, indices[i]); });
}
Acad::ErrorStatus EditEntity::SetLayer(const AcDbObjectId& id, const TCHAR* name)
{
    if (name == nullptr || *name == 0) return Acad::eInvalidInput;
    return edit(id, [name](AcDbEntity* entity)
    {
        AcDbDatabase* database = entity->database();
        if (database == nullptr) return Acad::eNoDatabase;
        AcDbLayerTable* table_ptr = nullptr;
        Acad::ErrorStatus status = database->getLayerTable(table_ptr, AcDb::kForWrite);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbLayerTable> table(table_ptr);
        if (!table->has(name))
        {
            Pola::DbObjectOwner<AcDbLayerTableRecord> record(new AcDbLayerTableRecord());
            status = record->setName(name);
            if (status == Acad::eOk) status = table->add(record.get());
            if (status == Acad::eOk) status = record.release()->close();
        }
        status = Pola::Close(table, status);
        if (status != Acad::eOk) return status;
        return entity->setLayer(name);
    });
}
Acad::ErrorStatus EditEntity::SetLayer(const AcDbObjectIdArray& ids, const TCHAR* name)
{
    return each(ids, [name](const AcDbObjectId& id, int) { return SetLayer(id, name); });
}
Acad::ErrorStatus EditEntity::SetLayer(const AcDbObjectIdArray& ids, const TCHAR* names[])
{
    if (names == nullptr) return Acad::eInvalidInput;
    return each(ids, [names](const AcDbObjectId& id, int i) { return SetLayer(id, names[i]); });
}
Acad::ErrorStatus EditEntity::SetLinetype(const AcDbObjectId& id, const TCHAR* name, const double& scale)
{
    if (name == nullptr || *name == 0 || !std::isfinite(scale) || scale <= 0) return Acad::eInvalidInput;
    return edit(id, [name, scale](AcDbEntity* entity)
    {
        AcDbDatabase* database = entity->database();
        if (database == nullptr) return Acad::eNoDatabase;
        AcDbLinetypeTable* table_ptr = nullptr;
        Acad::ErrorStatus status = database->getLinetypeTable(table_ptr, AcDb::kForRead);
        if (status != Acad::eOk) return status;
        Pola::OpenedObject<AcDbLinetypeTable> table(table_ptr);
        const bool exists = table->has(name);
        status = Pola::Close(table);
        if (status == Acad::eOk && !exists) status = database->loadLineTypeFile(name, _T("acad.lin"));
        if (status == Acad::eOk) status = entity->setLinetype(name);
        if (status == Acad::eOk) status = entity->setLinetypeScale(scale);
        return status;
    });
}
Acad::ErrorStatus EditEntity::SetLinetype(const AcDbObjectIdArray& ids, const TCHAR* name, const double& scale)
{
    return each(ids, [name, scale](const AcDbObjectId& id, int) { return SetLinetype(id, name, scale); });
}
Acad::ErrorStatus EditEntity::SetTextHorzMode(const AcDbObjectId& id, AcDb::TextHorzMode mode)
{
    return edit(id, [mode](AcDbEntity* entity)
    {
        AcDbText* text = AcDbText::cast(entity); // borrowed alias, close entity only
        if (text == nullptr) return Acad::eWrongObjectType;
        Acad::ErrorStatus status = text->setHorizontalMode(mode);
        if (status == Acad::eOk) status = text->setAlignmentPoint(text->position());
        if (status == Acad::eOk) status = text->adjustAlignment(text->database());
        return status;
    });
}
Acad::ErrorStatus EditEntity::SetTextHorzMode(const AcDbObjectIdArray& ids, AcDb::TextHorzMode mode)
{
    return each(ids, [mode](const AcDbObjectId& id, int) { return SetTextHorzMode(id, mode); });
}
Acad::ErrorStatus EditEntity::MoveEntity(const AcDbObjectId& id, const AcGePoint3d& base, const AcGePoint3d& target)
{
    if (!finitePoint(base) || !finitePoint(target)) return Acad::eInvalidInput;
    AcGeMatrix3d transform;
    transform.setToTranslation(target - base);
    return edit(id, [&transform](AcDbEntity* entity) { return entity->transformBy(transform); });
}
Acad::ErrorStatus EditEntity::MoveEntity(const AcDbObjectIdArray& ids, const AcGePoint3d& base, const AcGePoint3d& target)
{
    return each(ids, [&base, &target](const AcDbObjectId& id, int) { return MoveEntity(id, base, target); });
}
Acad::ErrorStatus EditEntity::MoveEntity(const AcDbObjectIdArray& ids, const AcGePoint3dArray& bases, const AcGePoint3dArray& targets)
{
    if (ids.length() != bases.length() || ids.length() != targets.length()) return Acad::eInvalidInput;
    return each(ids, [&bases, &targets](const AcDbObjectId& id, int i) { return MoveEntity(id, bases.at(i), targets.at(i)); });
}
Acad::ErrorStatus EditEntity::CopyEntity(const AcDbObjectId& id, const AcGePoint3d& base, const AcGePoint3d& target, AcDbObjectId& copy_id)
{
    copy_id = AcDbObjectId::kNull;
    if (id.isNull() || !finitePoint(base) || !finitePoint(target)) return Acad::eInvalidInput;
    AcDbEntity* base_ptr = nullptr;
    Acad::ErrorStatus status = acdbOpenObject(base_ptr, id, AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbEntity> original(base_ptr);
    AcGeMatrix3d transform;
    transform.setToTranslation(target - base);
    AcDbEntity* copy_ptr = nullptr;
    status = original->getTransformedCopy(transform, copy_ptr);
    Pola::DbObjectOwner<AcDbEntity> copy(copy_ptr);
    status = Pola::Close(original, status);
    if (status != Acad::eOk) return status;
    if (!copy) return Acad::eNullEntityPointer;
    return appendOwned(copy, copy_id);
}
AcDbObjectId EditEntity::CopyEntity(const AcDbObjectId& id, const AcGePoint3d& base, const AcGePoint3d& target)
{
    AcDbObjectId output;
    report(_T("CopyEntity"), CopyEntity(id, base, target, output));
    return output;
}
AcDbObjectIdArray EditEntity::CopyEntity(const AcDbObjectIdArray& ids, const AcGePoint3d& base, const AcGePoint3d& target)
{
    AcDbObjectIdArray output;
    for (int i = 0; i < ids.length(); ++i) output.append(CopyEntity(ids.at(i), base, target));
    return output;
}
AcDbObjectIdArray EditEntity::CopyEntity(const AcDbObjectIdArray& ids, const AcGePoint3dArray& bases, const AcGePoint3dArray& targets)
{
    AcDbObjectIdArray output;
    if (ids.length() != bases.length() || ids.length() != targets.length())
    {
        report(_T("CopyEntity"), Acad::eInvalidInput);
        return output;
    }
    for (int i = 0; i < ids.length(); ++i) output.append(CopyEntity(ids.at(i), bases.at(i), targets.at(i)));
    return output;
}
Acad::ErrorStatus EditEntity::RotateEntity(const AcDbObjectId& id, const AcGePoint3d& base, const double& angle, const int& type)
{
    if (!finitePoint(base) || !std::isfinite(angle) || (type != 0 && type != 1)) return Acad::eInvalidInput;
    AcGeMatrix3d transform;
    transform.setToRotation(type == 0 ? BasicTools::ConvertAngle(angle, 1) : angle, AcGeVector3d::kZAxis, base);
    return edit(id, [&transform](AcDbEntity* entity) { return entity->transformBy(transform); });
}
Acad::ErrorStatus EditEntity::RotateEntity(const AcDbObjectIdArray& ids, const AcGePoint3d& base, const double& angle, const int& type)
{
    return each(ids, [&base, angle, type](const AcDbObjectId& id, int) { return RotateEntity(id, base, angle, type); });
}
Acad::ErrorStatus EditEntity::RotateEntity(const AcDbObjectIdArray& ids, const AcGePoint3dArray& bases, const double angles[], const int& type)
{
    if (angles == nullptr || ids.length() != bases.length()) return Acad::eInvalidInput;
    return each(ids, [&bases, angles, type](const AcDbObjectId& id, int i) { return RotateEntity(id, bases.at(i), angles[i], type); });
}
Acad::ErrorStatus EditEntity::DeleteEntity(const AcDbObjectId& id)
{
    return edit(id, [](AcDbEntity* entity) { return entity->erase(); });
}
Acad::ErrorStatus EditEntity::DeleteEntity(const AcDbObjectIdArray& ids)
{
    return each(ids, [](const AcDbObjectId& id, int) { return DeleteEntity(id); });
}
Acad::ErrorStatus EditEntity::ScaleEntity(const AcDbObjectId& id, const AcGePoint3d& base, const double& scale)
{
    if (!finitePoint(base) || !std::isfinite(scale) || scale <= 0) return Acad::eInvalidInput;
    AcGeMatrix3d transform;
    transform.setToScaling(scale, base);
    return edit(id, [&transform](AcDbEntity* entity) { return entity->transformBy(transform); });
}
Acad::ErrorStatus EditEntity::ScaleEntity(const AcDbObjectIdArray& ids, const AcGePoint3d& base, const double& scale)
{
    return each(ids, [&base, scale](const AcDbObjectId& id, int) { return ScaleEntity(id, base, scale); });
}
Acad::ErrorStatus EditEntity::ScaleEntity(const AcDbObjectIdArray& ids, const AcGePoint3dArray& bases, const double& scale)
{
    if (ids.length() != bases.length()) return Acad::eInvalidInput;
    return each(ids, [&bases, scale](const AcDbObjectId& id, int i) { return ScaleEntity(id, bases.at(i), scale); });
}
Acad::ErrorStatus EditEntity::MirrorEntity(const AcDbObjectId& id, const AcGePoint3d& p1, const AcGePoint3d& p2, bool erase_original, AcDbObjectId& mirror_id)
{
    mirror_id = AcDbObjectId::kNull;
    if (id.isNull() || !finitePoint(p1) || !finitePoint(p2) || p1.isEqualTo(p2)) return Acad::eInvalidInput;
    AcDbEntity* base_ptr = nullptr;
    Acad::ErrorStatus status = acdbOpenObject(base_ptr, id, erase_original ? AcDb::kForWrite : AcDb::kForRead);
    if (status != Acad::eOk) return status;
    Pola::OpenedObject<AcDbEntity> original(base_ptr);
    AcGeMatrix3d transform;
    transform.setToMirroring(AcGeLine3d(p1, p2));
    AcDbEntity* mirror_ptr = nullptr;
    status = original->getTransformedCopy(transform, mirror_ptr);
    Pola::DbObjectOwner<AcDbEntity> mirror(mirror_ptr);
    if (status != Acad::eOk) return Pola::Close(original, status);
    if (!mirror) return Pola::Close(original, Acad::eNullEntityPointer);
    AcDbObjectId inserted_id;
    status = appendOwned(mirror, inserted_id);
    // Do not destroy the original if copying or insertion failed.
    if (status == Acad::eOk && erase_original) status = original->erase();
    status = Pola::Close(original, status);
    if (status == Acad::eOk) mirror_id = inserted_id;
    else if (!inserted_id.isNull()) DeleteEntity(inserted_id); // rollback, keep earliest error
    return status;
}
AcDbObjectId EditEntity::MirrorEntity(const AcDbObjectId& id, const AcGePoint3d& p1, const AcGePoint3d& p2, const bool erase_original)
{
    AcDbObjectId output;
    report(_T("MirrorEntity"), MirrorEntity(id, p1, p2, erase_original, output));
    return output;
}
AcDbObjectIdArray EditEntity::MirrorEntity(const AcDbObjectIdArray& ids, const AcGePoint3d& p1, const AcGePoint3d& p2, const bool erase_original)
{
    AcDbObjectIdArray output;
    for (int i = 0; i < ids.length(); ++i) output.append(MirrorEntity(ids.at(i), p1, p2, erase_original));
    return output;
}
