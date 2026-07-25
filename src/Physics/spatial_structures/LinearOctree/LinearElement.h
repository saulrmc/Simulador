#pragma once
#ifndef SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#define SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#include <bmi2intrin.h>
#include "Vec3.h"
#include <cstdint>

class LinearElement {
public:
    LinearElement();
    LinearElement(int bodyIndex);

    int get_body_index() const;
    void set_body_index(const int body_index);
    unsigned long long int get_morton_code() const;
    void calculate_morton_code(
        const Vec3& position, const Vec3& minPos, int resolutionLevel, double size);
    friend class LinearOctree;

private:
    int bodyIndex;
    unsigned long long int mortonCode;
};

inline LinearElement::LinearElement() : bodyIndex(-1), mortonCode(0) {}

inline LinearElement::LinearElement(int bodyIndex) : bodyIndex(bodyIndex), mortonCode(0) {}

inline int LinearElement::get_body_index() const {
    return bodyIndex;
}

inline void LinearElement::set_body_index(const int body_index) {
    bodyIndex = body_index;
}

inline unsigned long long int LinearElement::get_morton_code() const {
    return mortonCode;
}

inline void LinearElement::calculate_morton_code(
    const Vec3& position, const Vec3& minPos, int resolutionLevel, double size) {
    double normX = ((position.get_x() - minPos.get_x()) / size);
    double normY = ((position.get_y() - minPos.get_y()) / size);
    double normZ = ((position.get_z() - minPos.get_z()) / size);

    uint32_t discrX = static_cast<uint32_t>(normX * (1 << resolutionLevel));
    uint32_t discrY = static_cast<uint32_t>(normY * (1 << resolutionLevel));
    uint32_t discrZ = static_cast<uint32_t>(normZ * (1 << resolutionLevel));

    uint64_t mx = _pdep_u64(discrX, 0x1249249249249249ULL);
    uint64_t my = _pdep_u64(discrY, 0x2492492492492492ULL);
    uint64_t mz = _pdep_u64(discrZ, 0x4924924924924924ULL);

    mortonCode = mx | my | mz;
}

#endif
