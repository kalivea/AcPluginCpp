#include "../CustomPillar/PillarDwgFields.h"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <variant>

using Token = std::variant<Adesk::UInt32, Adesk::Int32, double, AcGePoint3d, AcGeVector3d, bool, std::wstring>;
struct Filer {
    std::vector<Token> tokens;
    size_t position = 0, failAt = static_cast<size_t>(-1);
    Acad::ErrorStatus terminal = Acad::eOk;
    template<class T> Acad::ErrorStatus readItem(T* value) {
        if (position == failAt || position >= tokens.size()) return Acad::eEndOfFile;
        const T* token = std::get_if<T>(&tokens[position++]);
        if (!token) return Acad::eFileAccessErr;
        *value = *token; return Acad::eOk;
    }
    Acad::ErrorStatus readUInt32(Adesk::UInt32* value) { return readItem(value); }
    Acad::ErrorStatus readString(std::wstring& value) { return readItem(&value); }
    template<class T> Acad::ErrorStatus writeItem(const T& value) {
        if (tokens.size() == failAt) return Acad::eFileAccessErr;
        tokens.emplace_back(value); return Acad::eOk;
    }
    Acad::ErrorStatus writeItem(const wchar_t* value) { return writeItem(std::wstring(value)); }
    Acad::ErrorStatus writeUInt32(Adesk::UInt32 value) { return writeItem(value); }
    Acad::ErrorStatus filerStatus() const { return terminal; }
};
static int checks = 0;
static void Check(bool success, const char* message) {
    ++checks;
    if (!success) { std::cerr << message << '\n'; std::exit(1); }
}
static Pola::PillarDwgFields Sample(int type = 1) {
    Pola::PillarDwgFields value;
    value.type = type; value.center = {20, 30, 0}; value.direction = {1, 0, 0};
    value.diameter = 2; value.height = type == 1 ? 4 : 0; value.property = 1;
    value.grade = 30; value.serial = L"P-1";
    if (type == 1) {
        value.vertices.append({19, 32, 0}); value.vertices.append({19, 28, 0});
        value.vertices.append({21, 28, 0}); value.vertices.append({21, 32, 0});
    }
    return value;
}
static void Unchanged(const Pola::PillarDwgFields& value) {
    Check(value.type == 0 && value.diameter == 17 && value.height == 0 && value.vertices.length() == 0
        && value.serial == L"KEEP" && value.grade == 30 && value.direction.x == 1,
        "failed read partially changed custom fields");
}
static Pola::PillarDwgFields Destination() {
    auto value = Sample(0); value.diameter = 17; value.serial = L"KEEP"; return value;
}
template<class T> static void Raw(std::ostream& stream, const T& value) {
    stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}
static void Record(std::ostream& stream, const Pola::PillarDwgFields& value) {
    Raw(stream, value.center.x); Raw(stream, value.center.y); Raw(stream, value.center.z);
    Raw(stream, value.direction.x); Raw(stream, value.direction.y); Raw(stream, value.direction.z);
    Raw(stream, value.diameter); Raw(stream, value.height); Raw(stream, value.visible); Raw(stream, value.property);
    Raw(stream, value.serial.length());
    stream.write(reinterpret_cast<const char*>(value.serial.data()), static_cast<std::streamsize>(value.serial.length() * sizeof(wchar_t)));
    Raw(stream, value.type); Raw(stream, static_cast<Adesk::Int32>(value.vertices.length()));
    for (int i = 0; i < value.vertices.length(); ++i) {
        Raw(stream, value.vertices[i].x); Raw(stream, value.vertices[i].y); Raw(stream, value.vertices[i].z);
    }
    Raw(stream, value.grade);
}
static std::string Binary(const Pola::PillarDwgFields& value) {
    std::ostringstream stream(std::ios::binary); Raw(stream, Adesk::Int32(1)); Record(stream, value); return stream.str();
}
template<class T> static void Replace(std::string& bytes, size_t offset, const T& value) {
    bytes.replace(offset, sizeof(T), reinterpret_cast<const char*>(&value), sizeof(T));
}
int main() {
    auto original = Sample();
    Filer encoded;
    Check(Pola::WritePillarFields(&encoded, original) == Acad::eOk, "valid v1 write failed");
    std::string layout;
    for (const auto& token : encoded.tokens) layout += "UIDPVBS"[token.index()];
    Check(layout == "UIPVDDBISPPPPI", "legacy v1 sequence changed");
    auto destination = Destination();
    Filer reader = encoded;
    Check(Pola::ReadPillarFields(&reader, destination) == Acad::eOk && destination.vertices.length() == 4,
        "valid read failed");
    reader.position = 0;
    Check(Pola::ReadPillarFields(&reader, destination) == Acad::eOk && destination.vertices.length() == 4,
        "repeated read appended vertices");
    Filer circle;
    Check(Pola::WritePillarFields(&circle, Sample(0)) == Acad::eOk, "legacy circle h=0 rejected");
    Check(Pola::ReadPillarFields(&circle, destination) == Acad::eOk && destination.vertices.length() == 0,
        "circle read retained previous rectangle vertices");
    for (size_t failure = 0; failure < encoded.tokens.size(); ++failure) {
        destination = Destination(); reader = encoded; reader.failAt = failure;
        Check(Pola::ReadPillarFields(&reader, destination) == Acad::eEndOfFile, "SDK read error not propagated");
        Unchanged(destination);
        Filer writer; writer.failAt = failure;
        Check(Pola::WritePillarFields(&writer, original) == Acad::eFileAccessErr, "SDK write error lost");
    }
    destination = Destination(); reader = encoded; reader.terminal = Acad::eFileAccessErr;
    Check(Pola::ReadPillarFields(&reader, destination) == Acad::eFileAccessErr, "terminal read failure ignored");
    Unchanged(destination);
    for (Adesk::UInt32 version : {0u, 2u, std::numeric_limits<Adesk::UInt32>::max()}) {
        reader = encoded; reader.tokens[0] = version;
        Check(Pola::ReadPillarFields(&reader, destination) == Acad::eMakeMeProxy, "unknown version accepted");
        Unchanged(destination);
    }
    for (size_t index : {size_t(1), size_t(7), size_t(13)}) {
        reader = encoded; reader.tokens[index] = Adesk::Int32(index == 13 ? -1 : 2);
        Check(Pola::ReadPillarFields(&reader, destination) == Acad::eInvalidInput, "invalid type/property/grade accepted");
        Unchanged(destination);
    }
    for (size_t index = 2; index < encoded.tokens.size(); ++index) {
        reader = encoded;
        if (auto* point = std::get_if<AcGePoint3d>(&reader.tokens[index])) point->x = std::numeric_limits<double>::infinity();
        else if (auto* vector = std::get_if<AcGeVector3d>(&reader.tokens[index])) vector->x = std::numeric_limits<double>::quiet_NaN();
        else if (auto* number = std::get_if<double>(&reader.tokens[index])) *number = std::numeric_limits<double>::quiet_NaN();
        else continue;
        Check(Pola::ReadPillarFields(&reader, destination) == Acad::eInvalidInput, "nonfinite geometry accepted");
        Unchanged(destination);
    }
    reader = encoded; reader.tokens[3] = AcGeVector3d{};
    Check(Pola::ReadPillarFields(&reader, destination) == Acad::eInvalidInput, "zero direction accepted");
    reader = encoded; reader.tokens[10] = original.vertices[0];
    Check(Pola::ReadPillarFields(&reader, destination) == Acad::eInvalidInput, "degenerate rectangle accepted");
    Unchanged(destination);
    for (size_t index : {size_t(4), size_t(5)}) {
        for (double dimension : {0.0, -1.0}) {
            reader = encoded; reader.tokens[index] = dimension;
            Check(Pola::ReadPillarFields(&reader, destination) == Acad::eInvalidInput, "invalid rectangle dimension accepted");
            Unchanged(destination);
        }
    }
    reader = encoded; reader.tokens[8] = std::wstring(Pola::kMaxPillarSerialLength + 1, L'x');
    Check(Pola::ReadPillarFields(&reader, destination) == Acad::eInvalidInput, "oversized serial accepted");
    Unchanged(destination);
    Check(Pola::ReadPillarFields<Filer>(nullptr, destination) == Acad::eInvalidInput, "null filer accepted");

    const std::string binary = Binary(original);
    bool found = false;
    std::istringstream stream(binary, std::ios::binary);
    Check(Pola::ReadPillarBinaryBySerial(stream, L"P-1", destination, found) == Acad::eOk && found
        && destination.vertices.length() == 4 && destination.direction.x == 1, "legacy binary round trip failed");
    for (size_t size = 0; size < binary.length(); ++size) {
        destination = Destination(); found = true;
        std::istringstream truncated(binary.substr(0, size), std::ios::binary);
        Check(Pola::ReadPillarBinaryBySerial(truncated, L"P-1", destination, found) == Acad::eEndOfFile
            && !found, "truncated binary record accepted");
        Unchanged(destination);
    }
    for (Adesk::Int32 count : {-1, Pola::kMaxPillarRecords + 1}) {
        auto bytes = binary; Replace(bytes, 0, count);
        std::istringstream corrupt(bytes, std::ios::binary);
        Check(Pola::ReadPillarBinaryBySerial(corrupt, L"P-1", destination, found) == Acad::eInvalidInput,
            "hostile binary record count accepted"); Unchanged(destination);
    }
    for (auto change : {0, 1, 2}) {
        auto bytes = binary;
        const size_t visible_offset = sizeof(Adesk::Int32) + 8 * sizeof(double);
        if (change == 0) Replace(bytes, visible_offset, static_cast<unsigned char>(2));
        if (change == 1) Replace(bytes, visible_offset + 1 + sizeof(Adesk::Int32), Pola::kMaxPillarSerialLength + 1);
        if (change == 2) Replace(bytes, visible_offset + 1 + sizeof(Adesk::Int32) + sizeof(size_t)
            + original.serial.length() * sizeof(wchar_t) + sizeof(Adesk::Int32), Adesk::Int32(-1));
        std::istringstream corrupt(bytes, std::ios::binary);
        Check(Pola::ReadPillarBinaryBySerial(corrupt, L"P-1", destination, found) == Acad::eInvalidInput,
            "invalid binary flag/length/vertices accepted"); Unchanged(destination);
    }
    destination = Destination(); found = true;
    std::istringstream unmatched(binary, std::ios::binary);
    Check(Pola::ReadPillarBinaryBySerial(unmatched, L"OTHER", destination, found) == Acad::eOk && !found,
        "unmatched serial changed found status"); Unchanged(destination);
    auto first = Sample(0); first.serial = L"FIRST";
    auto second = Sample(1); second.serial = L"SECOND"; second.direction = {0, 1, 0};
    std::ostringstream multiple(std::ios::binary); Raw(multiple, Adesk::Int32(2));
    Record(multiple, first); Record(multiple, second);
    std::istringstream search(multiple.str(), std::ios::binary);
    Check(Pola::ReadPillarBinaryBySerial(search, L"SECOND", destination, found) == Acad::eOk && found
        && destination.type == 1 && destination.direction.y == 1 && destination.vertices.length() == 4,
        "multiple-record binary search lost direction/vertices");
    destination = Destination();
    auto nonfinite = Sample(); nonfinite.center.x = std::numeric_limits<double>::quiet_NaN();
    std::istringstream invalid_binary(Binary(nonfinite), std::ios::binary);
    Check(Pola::ReadPillarBinaryBySerial(invalid_binary, L"P-1", destination, found) == Acad::eInvalidInput
        && !found, "nonfinite binary geometry accepted"); Unchanged(destination);
    auto copy = original; copy.serial = L"changed";
    Check(original.serial == L"P-1", "serial storage shared after copying");
    std::cout << "Pillar persistence host checks passed: " << checks << '\n';
}
