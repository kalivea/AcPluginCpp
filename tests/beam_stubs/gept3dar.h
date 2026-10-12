#pragma once
#include <cmath>
#include <vector>
struct AcGePoint3d {
    double x = 0, y = 0, z = 0;
    bool isEqualTo(const AcGePoint3d& value) const {
        return std::abs(x - value.x) < 1e-10 && std::abs(y - value.y) < 1e-10 && std::abs(z - value.z) < 1e-10;
    }
};
template<class T> struct BeamTestArray {
    std::vector<T> values;
    int length() const { return static_cast<int>(values.size()); }
    void setLogicalLength(int count) { values.resize(count); }
    void append(const T& value) { values.push_back(value); }
    T& operator[](int index) { return values.at(index); }
    const T& operator[](int index) const { return values.at(index); }
};
using AcGePoint3dArray = BeamTestArray<AcGePoint3d>;
