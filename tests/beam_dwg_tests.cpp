#include "../CustomBeam/BeamDwgFields.h"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <variant>

using Token = std::variant<Adesk::UInt32, Adesk::Int32, double, AcGePoint3d, AcGeVector3d>;
struct Filer {
    std::vector<Token> tokens;
    size_t position = 0, failAt = static_cast<size_t>(-1);
    Acad::ErrorStatus terminal = Acad::eOk;
    template<class T> Acad::ErrorStatus readItem(T* value) {
        if (position == failAt || position >= tokens.size()) return Acad::eEndOfFile;
        const T* token = std::get_if<T>(&tokens[position++]);
        if (!token) return Acad::eFileAccessErr;
        *value = *token;
        return Acad::eOk;
    }
    Acad::ErrorStatus readUInt32(Adesk::UInt32* value) { return readItem(value); }
    template<class T> Acad::ErrorStatus writeItem(T value) {
        if (tokens.size() == failAt) return Acad::eFileAccessErr;
        tokens.emplace_back(value);
        return Acad::eOk;
    }
    Acad::ErrorStatus writeUInt32(Adesk::UInt32 value) { return writeItem(value); }
    Acad::ErrorStatus filerStatus() const { return terminal; }
};
static int checks = 0;
static void Check(bool success, const char* message) {
    ++checks;
    if (!success) { std::cerr << message << '\n'; std::exit(1); }
}
static Pola::BeamDwgFields Sample(int count = 2) {
    Pola::BeamDwgFields value;
    value.count = count; value.width = 2; value.height = 4; value.property = 17;
    for (int i = 0; i < count; ++i) {
        value.vertices.append({10.0 * i, 0, 0});
        value.top.append({10.0 * i, 1, 0});
        value.bottom.append({10.0 * i, -1, 0});
        value.visible.push_back(i == 0 ? 0 : 1);
        value.directions.append({i == 0 ? 0.0 : 10.0, 0, 0});
    }
    return value;
}
static void Unchanged(const Pola::BeamDwgFields& value) {
    Check(value.count == 3 && value.width == 2 && value.height == 4 && value.property == 17 &&
        value.vertices.length() == 3 && value.vertices[2].x == 20 && value.visible.size() == 3,
        "failed read changed existing custom fields");
    const auto expected = Sample(3);
    Check(value.top.length() == 3 && value.bottom.length() == 3 && value.directions.length() == 3,
        "failed read changed derived arrays");
    for (int i = 0; i < 3; ++i)
        Check(value.vertices[i].isEqualTo(expected.vertices[i]) && value.top[i].isEqualTo(expected.top[i]) &&
            value.bottom[i].isEqualTo(expected.bottom[i]) && value.visible[i] == expected.visible[i] &&
            value.directions[i].x == expected.directions[i].x && value.directions[i].y == expected.directions[i].y &&
            value.directions[i].z == expected.directions[i].z, "failed read changed custom array contents");
}
int main() {
    auto original = Sample();
    Filer encoded;
    Check(Pola::WriteBeamFields(&encoded, original) == Acad::eOk, "valid v1 write failed");
    // Independently specify the legacy layout, including its unused visibility and direction slots.
    std::string actual;
    for (const auto& token : encoded.tokens) actual += "UIDPV"[token.index()];
    Check(actual == "UIPPPPPPDDIIIVV", "v1 field sequence changed");
    Pola::BeamDwgFields destination = Sample(3);
    Filer reading = encoded;
    Check(Pola::ReadBeamFields(&reading, destination) == Acad::eOk && destination.count == 2,
        "valid read failed or appended previous data");
    reading.position = 0;
    Check(Pola::ReadBeamFields(&reading, destination) == Acad::eOk && destination.vertices.length() == 2 &&
        destination.visible.size() == 2, "repeated read appended data");
    for (size_t fail = 0; fail < encoded.tokens.size(); ++fail) {
        destination = Sample(3);
        reading = encoded; reading.failAt = fail;
        Check(Pola::ReadBeamFields(&reading, destination) == Acad::eEndOfFile, "SDK read error not propagated");
        Unchanged(destination);
        Filer writing; writing.failAt = fail;
        Check(Pola::WriteBeamFields(&writing, original) == Acad::eFileAccessErr,
            "SDK write error not propagated");
    }
    destination = Sample(3);
    reading = encoded; reading.terminal = Acad::eFileAccessErr;
    Check(Pola::ReadBeamFields(&reading, destination) == Acad::eFileAccessErr, "terminal filer error lost");
    Unchanged(destination);
    for (Adesk::UInt32 version : {0u, 2u, std::numeric_limits<Adesk::UInt32>::max()}) {
        reading = encoded; reading.tokens[0] = version;
        Check(Pola::ReadBeamFields(&reading, destination) == Acad::eMakeMeProxy, "unknown version accepted");
        Unchanged(destination);
    }
    for (Adesk::Int32 count : {-1, 0, 1, Pola::kMaxBeamVertices + 1, std::numeric_limits<Adesk::Int32>::max()}) {
        reading = encoded; reading.tokens[1] = count;
        Check(Pola::ReadBeamFields(&reading, destination) == Acad::eInvalidInput && reading.position == 2,
            "hostile count allocated or accepted");
        Unchanged(destination);
    }
    for (size_t index = 2; index < encoded.tokens.size(); ++index) {
        reading = encoded;
        if (auto* point = std::get_if<AcGePoint3d>(&reading.tokens[index])) point->x = std::numeric_limits<double>::infinity();
        else if (auto* vector = std::get_if<AcGeVector3d>(&reading.tokens[index])) vector->z = std::numeric_limits<double>::quiet_NaN();
        else if (auto* number = std::get_if<double>(&reading.tokens[index])) *number = std::numeric_limits<double>::quiet_NaN();
        else continue;
        Check(Pola::ReadBeamFields(&reading, destination) == Acad::eInvalidInput, "nonfinite geometry accepted");
        Unchanged(destination);
    }
    for (size_t index : {size_t(10), size_t(11)}) {
        reading = encoded; reading.tokens[index] = Adesk::Int32(2);
        Check(Pola::ReadBeamFields(&reading, destination) == Acad::eInvalidInput, "invalid visibility accepted");
        Unchanged(destination);
    }
    for (size_t index : {size_t(8), size_t(9)}) {
        for (double value : {0.0, -1.0}) {
            reading = encoded; reading.tokens[index] = value;
            Check(Pola::ReadBeamFields(&reading, destination) == Acad::eInvalidInput, "invalid dimension accepted");
            Unchanged(destination);
        }
    }
    reading = encoded; reading.tokens[3] = std::get<AcGePoint3d>(reading.tokens[2]);
    Check(Pola::ReadBeamFields(&reading, destination) == Acad::eInvalidInput, "zero length segment accepted");
    Unchanged(destination);
    reading = encoded;
    reading.tokens[2] = AcGePoint3d{-1e308, 0, 0};
    reading.tokens[3] = AcGePoint3d{1e308, 0, 0};
    Check(Pola::ReadBeamFields(&reading, destination) == Acad::eInvalidInput,
        "overflowing finite coordinates accepted");
    Unchanged(destination);
    Filer terminalWriter; terminalWriter.terminal = Acad::eFileAccessErr;
    Check(Pola::WriteBeamFields(&terminalWriter, original) == Acad::eFileAccessErr,
        "terminal writer status lost");
    auto malformed = original; malformed.top.setLogicalLength(1);
    Filer writer;
    Check(Pola::WriteBeamFields(&writer, malformed) == Acad::eInvalidInput && writer.tokens.empty(),
        "inconsistent arrays written");
    Check(Pola::ReadBeamFields<Filer>(nullptr, destination) == Acad::eInvalidInput, "null read filer accepted");
    Check(Pola::WriteBeamFields<Filer>(nullptr, original) == Acad::eInvalidInput, "null write filer accepted");
    std::cout << "Beam DWG host checks passed: " << checks << '\n';
}

