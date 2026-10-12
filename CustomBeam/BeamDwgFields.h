#pragma once

#include "dbfiler.h"
#include "gept3dar.h"
#include "gevc3dar.h"
#include <cmath>
#include <new>
#include <utility>
#include <vector>

namespace Pola {

// A limit on hostile DWG allocations; v1 stores three point arrays and one
// direction per vertex. The serialized field order remains unchanged.
constexpr Adesk::Int32 kMaxBeamVertices = 100000;

struct BeamDwgFields {
    AcGePoint3dArray vertices, top, bottom;
    Adesk::Int32 count = 0;
    double width = 0.0, height = 0.0;
    std::vector<Adesk::Int32> visible;
    Adesk::Int32 property = 0;
    AcGeVector3dArray directions;
};

template<class PointOrVector>
inline bool BeamFinite(const PointOrVector& value) {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

inline Acad::ErrorStatus ValidateBeamFields(const BeamDwgFields& fields) {
    if (fields.count < 2 || fields.count > kMaxBeamVertices ||
        fields.vertices.length() != fields.count || fields.top.length() != fields.count ||
        fields.bottom.length() != fields.count || fields.directions.length() != fields.count ||
        fields.visible.size() != static_cast<size_t>(fields.count) ||
        !std::isfinite(fields.width) || fields.width <= 0.0 ||
        !std::isfinite(fields.height) || fields.height <= 0.0)
        return Acad::eInvalidInput;
    for (int i = 0; i < fields.count; ++i) {
        if (!BeamFinite(fields.vertices[i]) || !BeamFinite(fields.top[i]) ||
            !BeamFinite(fields.bottom[i]) || !BeamFinite(fields.directions[i]) ||
            (fields.visible[i] != 0 && fields.visible[i] != 1))
            return Acad::eInvalidInput;
        if (i > 0) {
            const auto& point = fields.vertices[i];
            const auto& previous = fields.vertices[i - 1];
            if (point.isEqualTo(previous) || !std::isfinite(point.x - previous.x) ||
                !std::isfinite(point.y - previous.y) || !std::isfinite(point.z - previous.z))
                return Acad::eInvalidInput;
        }
    }
    return Acad::eOk;
}

// The parent entity fields are handled by CPolaCustomBeam. Only commit these
// custom fields after the entire v1 record and filer status have passed.
template<class Filer>
Acad::ErrorStatus ReadBeamFields(Filer* filer, BeamDwgFields& destination) {
    if (!filer)
        return Acad::eInvalidInput;
    Adesk::UInt32 version = 0;
    Acad::ErrorStatus es = filer->readUInt32(&version);
    if (es != Acad::eOk)
        return es;
    if (version != 1)
        return Acad::eMakeMeProxy;
    BeamDwgFields value;
    if ((es = filer->readItem(&value.count)) != Acad::eOk)
        return es;
    if (value.count < 2 || value.count > kMaxBeamVertices)
        return Acad::eInvalidInput;
    try {
        value.vertices.setLogicalLength(value.count);
        value.top.setLogicalLength(value.count);
        value.bottom.setLogicalLength(value.count);
        value.directions.setLogicalLength(value.count);
        value.visible.resize(value.count);
        for (int i = 0; i < value.count; ++i)
            if ((es = filer->readItem(&value.vertices[i])) != Acad::eOk) return es;
        for (int i = 0; i < value.count; ++i)
            if ((es = filer->readItem(&value.top[i])) != Acad::eOk) return es;
        for (int i = 0; i < value.count; ++i)
            if ((es = filer->readItem(&value.bottom[i])) != Acad::eOk) return es;
        if ((es = filer->readItem(&value.width)) != Acad::eOk) return es;
        if ((es = filer->readItem(&value.height)) != Acad::eOk) return es;
        for (int i = 0; i < value.count; ++i)
            if ((es = filer->readItem(&value.visible[i])) != Acad::eOk) return es;
        if ((es = filer->readItem(&value.property)) != Acad::eOk) return es;
        for (int i = 0; i < value.count; ++i)
            if ((es = filer->readItem(&value.directions[i])) != Acad::eOk) return es;
        if ((es = filer->filerStatus()) != Acad::eOk) return es;
        if ((es = ValidateBeamFields(value)) != Acad::eOk) return es;
        destination = std::move(value);
        return Acad::eOk;
    } catch (const std::bad_alloc&) {
        return Acad::eOutOfMemory;
    }
}

template<class Filer>
Acad::ErrorStatus WriteBeamFields(Filer* filer, const BeamDwgFields& value) {
    if (!filer)
        return Acad::eInvalidInput;
    Acad::ErrorStatus es = ValidateBeamFields(value);
    if (es != Acad::eOk) return es;
    if ((es = filer->writeUInt32(1)) != Acad::eOk) return es;
    if ((es = filer->writeItem(value.count)) != Acad::eOk) return es;
    for (int i = 0; i < value.count; ++i)
        if ((es = filer->writeItem(value.vertices[i])) != Acad::eOk) return es;
    for (int i = 0; i < value.count; ++i)
        if ((es = filer->writeItem(value.top[i])) != Acad::eOk) return es;
    for (int i = 0; i < value.count; ++i)
        if ((es = filer->writeItem(value.bottom[i])) != Acad::eOk) return es;
    if ((es = filer->writeItem(value.width)) != Acad::eOk) return es;
    if ((es = filer->writeItem(value.height)) != Acad::eOk) return es;
    for (int i = 0; i < value.count; ++i)
        if ((es = filer->writeItem(value.visible[i])) != Acad::eOk) return es;
    if ((es = filer->writeItem(value.property)) != Acad::eOk) return es;
    for (int i = 0; i < value.count; ++i)
        if ((es = filer->writeItem(value.directions[i])) != Acad::eOk) return es;
    return filer->filerStatus();
}

} // namespace Pola
