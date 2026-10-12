#pragma once
#include <memory>

namespace Pola
{
// Owns one transient object, or one open database-object handle. Cast aliases
// and borrowed parameters must never be put into a second owner.
struct DbObjectDeleter
{
    template<class T> void operator()(T* object) const
    {
        if (object == nullptr) return;
        if (object->objectId().isNull()) delete object;
        else object->close();
    }
};
template<class T> using DbObjectOwner = std::unique_ptr<T, DbObjectDeleter>;

struct OpenedObjectDeleter
{
    template<class T> void operator()(T* object) const
    {
        if (object != nullptr) object->close();
    }
};
template<class T> using OpenedObject = std::unique_ptr<T, OpenedObjectDeleter>;

// Explicit close makes cleanup failures observable without hiding the first
// operation failure. Releasing before close guarantees one close attempt.
template<class T> Acad::ErrorStatus Close(OpenedObject<T>& object, Acad::ErrorStatus status = Acad::eOk)
{
    const Acad::ErrorStatus cleanup = object ? object.release()->close() : Acad::eOk;
    return status == Acad::eOk ? cleanup : status;
}
}
