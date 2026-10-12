#pragma once
#include "dbfiler.h"
#include "gept3dar.h"
#include "gevc3dar.h"
#include <cmath>
#include <istream>
#include <string>
#include <utility>

namespace Pola {
constexpr size_t kMaxPillarSerialLength = 65536;
constexpr Adesk::Int32 kMaxPillarRecords = 1000000;
struct PillarDwgFields {
    Adesk::Int32 type = 0;
    AcGePoint3d center;
    AcGeVector3d direction;
    double diameter = 0.0, height = 0.0;
    bool visible = true;
    Adesk::Int32 property = 0;
    std::wstring serial;
    AcGePoint3dArray vertices;
    Adesk::Int32 grade = 0;
};
inline bool FinitePillarPoint(const AcGePoint3d& point) {
    return std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z);
}
inline Acad::ErrorStatus ValidatePillarFields(const PillarDwgFields& value) {
    if ((value.type != 0 && value.type != 1) || (value.property != 0 && value.property != 1)
        || value.grade < 0 || value.serial.length() > kMaxPillarSerialLength
        || !FinitePillarPoint(value.center) || !std::isfinite(value.direction.x)
        || !std::isfinite(value.direction.y) || !std::isfinite(value.direction.z)
        || !std::isfinite(std::hypot(std::hypot(value.direction.x, value.direction.y), value.direction.z))
        || std::hypot(value.direction.x, value.direction.y) <= 0.0
        || !std::isfinite(value.diameter) || !std::isfinite(value.height)
        || value.diameter <= 0.0 || value.height < 0.0 || (value.type == 1 && value.height <= 0.0)
        || value.vertices.length() != (value.type == 1 ? 4 : 0)) return Acad::eInvalidInput;
    for (int i = 0; i < value.vertices.length(); ++i) {
        if (!FinitePillarPoint(value.vertices[i])) return Acad::eInvalidInput;
        for (int j = 0; j < i; ++j)
            if (value.vertices[i].isEqualTo(value.vertices[j])) return Acad::eInvalidInput;
    }
    return Acad::eOk;
}

// Filer reads must own strings; the ObjectARX adapter uses readString(AcString&).
// The v1 sequence deliberately stays identical to existing drawings.
template<class Filer> Acad::ErrorStatus ReadPillarFields(Filer* filer, PillarDwgFields& value) {
    if (!filer) return Acad::eInvalidInput;
    Adesk::UInt32 version = 0;
    Acad::ErrorStatus status = filer->readUInt32(&version);
    if (status != Acad::eOk) return status;
    if (version != 1) return Acad::eMakeMeProxy;
    PillarDwgFields next;
    if ((status = filer->readItem(&next.type)) != Acad::eOk) return status;
    if (next.type != 0 && next.type != 1) return Acad::eInvalidInput;
    if ((status = filer->readItem(&next.center)) != Acad::eOk) return status;
    if ((status = filer->readItem(&next.direction)) != Acad::eOk) return status;
    if ((status = filer->readItem(&next.diameter)) != Acad::eOk) return status;
    if ((status = filer->readItem(&next.height)) != Acad::eOk) return status;
    if ((status = filer->readItem(&next.visible)) != Acad::eOk) return status;
    if ((status = filer->readItem(&next.property)) != Acad::eOk) return status;
    if ((status = filer->readString(next.serial)) != Acad::eOk) return status;
    if (next.serial.length() > kMaxPillarSerialLength) return Acad::eInvalidInput;
    if (next.type == 1) {
        for (int i = 0; i < 4; ++i) {
            AcGePoint3d vertex;
            if ((status = filer->readItem(&vertex)) != Acad::eOk) return status;
            next.vertices.append(vertex);
        }
    }
    if ((status = filer->readItem(&next.grade)) != Acad::eOk) return status;
    if ((status = filer->filerStatus()) != Acad::eOk) return status;
    if ((status = ValidatePillarFields(next)) != Acad::eOk) return status;
    value = std::move(next);
    return Acad::eOk;
}
template<class Filer> Acad::ErrorStatus WritePillarFields(Filer* filer, const PillarDwgFields& value) {
    if (!filer) return Acad::eInvalidInput;
    Acad::ErrorStatus status = ValidatePillarFields(value);
    if (status != Acad::eOk) return status;
    if ((status = filer->writeUInt32(1)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.type)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.center)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.direction)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.diameter)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.height)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.visible)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.property)) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.serial.c_str())) != Acad::eOk) return status;
    for (int i = 0; i < value.vertices.length(); ++i)
        if ((status = filer->writeItem(value.vertices[i])) != Acad::eOk) return status;
    if ((status = filer->writeItem(value.grade)) != Acad::eOk) return status;
    return filer->filerStatus();
}

template<class T> bool ReadPillarBinaryValue(std::istream& stream, T& value) {
    return static_cast<bool>(stream.read(reinterpret_cast<char*>(&value), sizeof(T)));
}
inline Acad::ErrorStatus ReadPillarBinaryRecord(std::istream& stream, PillarDwgFields& value) {
    static_assert(sizeof(bool) == 1, "Legacy binary format requires a one-byte bool");
    PillarDwgFields next;
    if (!ReadPillarBinaryValue(stream, next.center.x) || !ReadPillarBinaryValue(stream, next.center.y)
        || !ReadPillarBinaryValue(stream, next.center.z) || !ReadPillarBinaryValue(stream, next.direction.x)
        || !ReadPillarBinaryValue(stream, next.direction.y) || !ReadPillarBinaryValue(stream, next.direction.z)
        || !ReadPillarBinaryValue(stream, next.diameter) || !ReadPillarBinaryValue(stream, next.height)) return Acad::eEndOfFile;
    unsigned char visible = 0;
    size_t serial_length = 0;
    if (!ReadPillarBinaryValue(stream, visible)) return Acad::eEndOfFile;
    if (visible > 1) return Acad::eInvalidInput;
    next.visible = visible != 0;
    if (!ReadPillarBinaryValue(stream, next.property) || !ReadPillarBinaryValue(stream, serial_length)) return Acad::eEndOfFile;
    if (serial_length > kMaxPillarSerialLength) return Acad::eInvalidInput;
    next.serial.resize(serial_length);
    if (serial_length && !stream.read(reinterpret_cast<char*>(&next.serial[0]), static_cast<std::streamsize>(serial_length * sizeof(wchar_t)))) return Acad::eEndOfFile;
    Adesk::Int32 count = 0;
    if (!ReadPillarBinaryValue(stream, next.type) || !ReadPillarBinaryValue(stream, count)) return Acad::eEndOfFile;
    if ((next.type != 0 && next.type != 1) || count != (next.type == 1 ? 4 : 0)) return Acad::eInvalidInput;
    for (int i = 0; i < count; ++i) {
        AcGePoint3d vertex;
        if (!ReadPillarBinaryValue(stream, vertex.x) || !ReadPillarBinaryValue(stream, vertex.y)
            || !ReadPillarBinaryValue(stream, vertex.z)) return Acad::eEndOfFile;
        next.vertices.append(vertex);
    }
    if (!ReadPillarBinaryValue(stream, next.grade)) return Acad::eEndOfFile;
    const Acad::ErrorStatus status = ValidatePillarFields(next);
    if (status != Acad::eOk) return status;
    value = std::move(next);
    return Acad::eOk;
}
inline Acad::ErrorStatus ReadPillarBinaryBySerial(std::istream& stream, const std::wstring& serial,
    PillarDwgFields& value, bool& found) {
    found = false;
    Adesk::Int32 count = 0;
    if (!ReadPillarBinaryValue(stream, count)) return Acad::eEndOfFile;
    if (count < 0 || count > kMaxPillarRecords || serial.length() > kMaxPillarSerialLength) return Acad::eInvalidInput;
    for (Adesk::Int32 i = 0; i < count; ++i) {
        PillarDwgFields next;
        const Acad::ErrorStatus status = ReadPillarBinaryRecord(stream, next);
        if (status != Acad::eOk) return status;
        if (next.serial == serial) { value = std::move(next); found = true; return Acad::eOk; }
    }
    return Acad::eOk;
}
} // namespace Pola
