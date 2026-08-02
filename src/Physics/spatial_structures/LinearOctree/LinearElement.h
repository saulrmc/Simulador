#pragma once
#ifndef SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#define SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#include <x86gprintrin.h>
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
    if (normX < 0) normX = 0;
    if (normX > 1) normX = 1;
    if (normY < 0) normY = 0;
    if (normY > 1) normY = 1;
    if (normZ < 0) normZ = 0;
    if (normZ > 1) normZ = 1;

    uint32_t maxVal = 1u << resolutionLevel;
    uint32_t discrX = static_cast<uint32_t>(normX * maxVal);
    uint32_t discrY = static_cast<uint32_t>(normY * maxVal);
    uint32_t discrZ = static_cast<uint32_t>(normZ * maxVal);
    if (discrX >= maxVal) discrX = maxVal - 1;
    if (discrY >= maxVal) discrY = maxVal - 1;
    if (discrZ >= maxVal) discrZ = maxVal - 1;

    uint64_t mx = _pdep_u64(discrX, 0x1249249249249249ULL);
    uint64_t my = _pdep_u64(discrY, 0x2492492492492492ULL);
    uint64_t mz = _pdep_u64(discrZ, 0x4924924924924924ULL);

    mortonCode = mx | my | mz;
}

#endif
