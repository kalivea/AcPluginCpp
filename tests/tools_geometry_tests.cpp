#include <cassert>
#include <cmath>
#include <cstdio>
#include <functional>
#include <limits>
#include <memory>
#include <new>
#include <vector>

namespace Acad { enum ErrorStatus { eOk, eInvalidInput, eOutOfMemory, eSdkError, ePointError, eVertexError }; }
struct AcGePoint2d { double x = 0, y = 0; };
struct AcGePoint3d {
    double x = 0, y = 0, z = 0;
    bool isEqualTo(const AcGePoint3d& other) const {
        return std::abs(x - other.x) < 1e-10 && std::abs(y - other.y) < 1e-10 && std::abs(z - other.z) < 1e-10;
    }
};
template<class T> struct TestArray {
    inline static int appendFailAfter = -1;
    inline static bool copyFails = false;
    std::vector<T> values;
    TestArray() = default;
    TestArray(const TestArray& other) {
        if (copyFails) throw std::bad_alloc();
        values = other.values;
    }
    TestArray& operator=(const TestArray& other) { values = other.values; return *this; }
    int length() const { return static_cast<int>(values.size()); }
    void removeAll() { values.clear(); }
    void append(const T& value) {
        if (appendFailAfter == static_cast<int>(values.size())) throw std::bad_alloc();
        values.push_back(value);
    }
    const T& first() const { return values.front(); }
    const T& last() const { return values.back(); }
    const T& operator[](int index) const { return values.at(index); }
};
using AcGePoint3dArray = TestArray<AcGePoint3d>;
using AcDbVoidPtrArray = TestArray<void*>;
struct AcDbEntity {
    inline static int offsetObjectsAlive = 0;
    int closes = 0;
    bool offsetObject = false;
    explicit AcDbEntity(bool offset = false) : offsetObject(offset) { if (offsetObject) ++offsetObjectsAlive; }
    virtual ~AcDbEntity() { if (offsetObject) --offsetObjectsAlive; }
    void close() { ++closes; }
};
struct AcDbLine : AcDbEntity {
    AcGePoint3d start, end;
    AcDbLine(AcGePoint3d first, AcGePoint3d second) : start(first), end(second) {}
    static const AcDbLine* cast(const AcDbEntity* entity) { return dynamic_cast<const AcDbLine*>(entity); }
    AcGePoint3d startPoint() const { return start; }
    AcGePoint3d endPoint() const { return end; }
};
struct AcDbPolyline : AcDbEntity {
    inline static int instances = 0, offsetCalls = 0, addFailureAt = -1;
    inline static Acad::ErrorStatus offsetStatus = Acad::eOk;
    inline static std::function<std::vector<AcDbEntity*>()> factory;
    std::vector<AcGePoint3d> points{{0,0,0},{10,0,0}};
    bool closed = false;
    int readFailureAt = -1;
    explicit AcDbPolyline(bool offset = false) : AcDbEntity(offset) { ++instances; }
    ~AcDbPolyline() override { --instances; }
    static const AcDbPolyline* cast(const AcDbEntity* entity) { return dynamic_cast<const AcDbPolyline*>(entity); }
    unsigned int numVerts() const { return static_cast<unsigned int>(points.size()); }
    bool isClosed() const { return closed; }
    Acad::ErrorStatus getPointAt(unsigned int index, AcGePoint3d& result) const {
        if (static_cast<int>(index) == readFailureAt) return Acad::ePointError;
        result = points.at(index);
        return Acad::eOk;
    }
    Acad::ErrorStatus addVertexAt(int index, AcGePoint2d point) {
        if (index == addFailureAt) return Acad::eVertexError;
        if (index == 0) points.clear();
        points.push_back({point.x, point.y, 0});
        return Acad::eOk;
    }
    Acad::ErrorStatus getOffsetCurves(double, AcDbVoidPtrArray& curves) const {
        ++offsetCalls;
        if (factory) for (AcDbEntity* entity : factory()) curves.append(entity);
        return offsetStatus;
    }
};
struct AcGeLineSeg3d {
    bool valid = false;
    AcGePoint3d start, end;
    void set(AcGePoint3d first, AcGePoint3d second) { start = first; end = second; valid = true; }
};
using AcGeLine3d = AcGeLineSeg3d;
class BasicTools {
public:
    static AcGePoint2d Point3dToPoint2d(AcGePoint3d point) { return {point.x, point.y}; }
    static Acad::ErrorStatus OffsetPolyLineChecked(const AcDbPolyline&, const double&, AcGePoint3dArray&);
    static Acad::ErrorStatus OffsetPolyLineChecked(const AcGePoint3dArray&, const double&, AcGePoint3dArray&);
    static Acad::ErrorStatus EntityToLineSegment(const AcDbEntity*, AcGeLineSeg3d&);
    static Acad::ErrorStatus EntityToLine(const AcDbEntity*, AcGeLine3d&);
    static AcGeLineSeg3d EntityToLineSegment(const AcDbEntity*);
    static AcGeLine3d EntityToLine(const AcDbEntity*);
};
#include "ToolsOffsetUnderTest.inc"
#include "ToolsBorrowedLineUnderTest.inc"

int main() {
    int cases = 0;
    AcDbLine borrowed({0,0,0}, {10,1,0});
    AcGeLineSeg3d segment; AcGeLine3d line;
    assert(BasicTools::EntityToLineSegment(&borrowed, segment) == Acad::eOk && segment.valid);
    assert(BasicTools::EntityToLine(&borrowed, line) == Acad::eOk && line.valid);
    assert(BasicTools::EntityToLineSegment(&borrowed).valid && BasicTools::EntityToLine(&borrowed).valid);
    assert(borrowed.closes == 0); ++cases;
    AcDbEntity other;
    assert(BasicTools::EntityToLineSegment(&other, segment) == Acad::eInvalidInput && !segment.valid);
    assert(BasicTools::EntityToLine(nullptr, line) == Acad::eInvalidInput && !line.valid);
    assert(other.closes == 0 && borrowed.closes == 0); ++cases;
    for (AcGePoint3d endpoint : {AcGePoint3d{}, AcGePoint3d{std::numeric_limits<double>::infinity(),0,0},
        AcGePoint3d{0,std::numeric_limits<double>::quiet_NaN(),0}}) {
        AcDbLine invalid({}, endpoint);
        assert(BasicTools::EntityToLineSegment(&invalid, segment) == Acad::eInvalidInput && !segment.valid);
        assert(BasicTools::EntityToLine(&invalid, line) == Acad::eInvalidInput && !line.valid);
        assert(invalid.closes == 0); ++cases;
    }
    AcDbLine overflow({-1e308,0,0},{1e308,0,0});
    assert(BasicTools::EntityToLine(&overflow,line) == Acad::eInvalidInput && overflow.closes == 0); ++cases;

    AcDbPolyline input;
    AcGePoint3dArray output;
    auto CheckOffset = [&](Acad::ErrorStatus expected) {
        output.values = {{99,99,99}};
        assert(BasicTools::OffsetPolyLineChecked(input, 2, output) == expected);
        assert(AcDbEntity::offsetObjectsAlive == 0 && input.closes == 0);
        if (expected != Acad::eOk) assert(output.length() == 0);
        ++cases;
    };
    AcDbPolyline::factory = [] { return std::vector<AcDbEntity*>{new AcDbPolyline(true)}; };
    CheckOffset(Acad::eOk); assert(output.length() == 2 && output[1].x == 10);
    AcDbPolyline::factory = [] { auto* value = new AcDbPolyline(true); value->closed = true;
        return std::vector<AcDbEntity*>{value}; };
    CheckOffset(Acad::eOk); assert(output.length() == 3 && output.first().isEqualTo(output.last()));
    AcDbPolyline::factory = [] { return std::vector<AcDbEntity*>{}; };
    CheckOffset(Acad::eInvalidInput);
    AcDbPolyline::factory = [] { return std::vector<AcDbEntity*>{new AcDbPolyline(true),new AcDbEntity(true)}; };
    CheckOffset(Acad::eInvalidInput);
    AcDbPolyline::offsetStatus = Acad::eSdkError;
    CheckOffset(Acad::eSdkError); // SDK may return partial curves on failure.
    AcDbPolyline::offsetStatus = Acad::eOk;
    AcDbPolyline::factory = [] { return std::vector<AcDbEntity*>{new AcDbEntity(true)}; };
    CheckOffset(Acad::eInvalidInput);
    AcDbPolyline::factory = [] { return std::vector<AcDbEntity*>{nullptr}; };
    CheckOffset(Acad::eInvalidInput);
    AcDbPolyline::factory = [] { auto* value = new AcDbPolyline(true); value->readFailureAt = 1;
        return std::vector<AcDbEntity*>{value}; };
    CheckOffset(Acad::ePointError);
    AcDbPolyline::factory = [] { auto* value = new AcDbPolyline(true); value->points[0].z = std::numeric_limits<double>::infinity();
        return std::vector<AcDbEntity*>{value}; };
    CheckOffset(Acad::eInvalidInput);
    AcDbPolyline::factory = [] { return std::vector<AcDbEntity*>{new AcDbPolyline(true)}; };
    AcGePoint3dArray::appendFailAfter = 1;
    CheckOffset(Acad::eOutOfMemory);
    AcGePoint3dArray::appendFailAfter = -1;
    const int beforeInvalid = AcDbPolyline::offsetCalls;
    for (double distance : {0.0,std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()}) {
        output.values = {{99,0,0}};
        assert(BasicTools::OffsetPolyLineChecked(input,distance,output) == Acad::eInvalidInput && output.length() == 0);
        ++cases;
    }
    assert(AcDbPolyline::offsetCalls == beforeInvalid);
    AcGePoint3dArray centers; centers.values = {{0,0,0},{10,0,0}};
    const int instances = AcDbPolyline::instances;
    assert(BasicTools::OffsetPolyLineChecked(centers,2,output) == Acad::eOk && output.length() == 2);
    assert(AcDbPolyline::instances == instances && AcDbEntity::offsetObjectsAlive == 0); ++cases;
    assert(BasicTools::OffsetPolyLineChecked(centers,2,centers) == Acad::eOk && centers.length() == 2); ++cases;
    AcDbPolyline::addFailureAt = 1;
    assert(BasicTools::OffsetPolyLineChecked(centers,2,output) == Acad::eVertexError && output.length() == 0);
    assert(AcDbPolyline::instances == instances); ++cases;
    AcDbPolyline::addFailureAt = -1;
    centers.values = {{0,0,0},{0,0,0}};
    assert(BasicTools::OffsetPolyLineChecked(centers,2,output) == Acad::eInvalidInput && output.length() == 0); ++cases;
    centers.values = {{0,0,0},{std::numeric_limits<double>::quiet_NaN(),0,0}};
    assert(BasicTools::OffsetPolyLineChecked(centers,2,output) == Acad::eInvalidInput && output.length() == 0); ++cases;
    centers.values = {{0,0,0},{10,0,0}};
    AcGePoint3dArray::copyFails = true;
    assert(BasicTools::OffsetPolyLineChecked(centers,2,output) == Acad::eOutOfMemory && output.length() == 0);
    AcGePoint3dArray::copyFails = false; ++cases;
    assert(AcDbPolyline::instances == instances && AcDbEntity::offsetObjectsAlive == 0);
    std::printf("Tools production geometry regression: %d cases passed\n",cases);
}
