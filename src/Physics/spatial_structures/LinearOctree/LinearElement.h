#pragma once
#ifndef SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#define SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#include "Vec3.h"
#include <cstdint>

class LinearElement {
public:
    LinearElement();
    LinearElement(int bodyIndex);

    int get_bodyIndex() const;

    unsigned long long int get_morton_code() const;

    void calculate_morton_code(const Vec3& position, const Vec3& minPos, double resolution);

private:
    int bodyIndex;
    unsigned long long int mortonCode;
};

inline LinearElement::LinearElement() : bodyIndex(-1), mortonCode(0) {}

inline LinearElement::LinearElement(int bodyIndex) : bodyIndex(bodyIndex), mortonCode(0) {}

inline int LinearElement::get_bodyIndex() const {
    return bodyIndex;
}

inline unsigned long long int LinearElement::get_morton_code() const {
    return mortonCode;
}

inline void LinearElement::calculate_morton_code(const Vec3& position, const Vec3& minPos, double resolution) {
    int x = static_cast<int>((position.get_x() - minPos.get_x()) / resolution);
    int y = static_cast<int>((position.get_y() - minPos.get_y()) / resolution);
    int z = static_cast<int>((position.get_z() - minPos.get_z()) / resolution);

    if (x < 0 || y < 0 || z < 0) {
        mortonCode = 0;
        return;
    }

    unsigned long long int ux = static_cast<unsigned long long int>(x) & 0x1FFFFF;
    unsigned long long int uy = static_cast<unsigned long long int>(y) & 0x1FFFFF;
    unsigned long long int uz = static_cast<unsigned long long int>(z) & 0x1FFFFF;

    ux = (ux | (ux << 32)) & 0x001F00000000FFFF;
    ux = (ux | (ux << 16)) & 0x001F0000FF0000FF;
    ux = (ux | (ux << 8))  & 0x100F00F00F00F00F;
    ux = (ux | (ux << 4))  & 0x10C30C30C30C30C3;
    ux = (ux | (ux << 2))  & 0x1249249249249249;

    uy = (uy | (uy << 32)) & 0x001F00000000FFFF;
    uy = (uy | (uy << 16)) & 0x001F0000FF0000FF;
    uy = (uy | (uy << 8))  & 0x100F00F00F00F00F;
    uy = (uy | (uy << 4))  & 0x10C30C30C30C30C3;
    uy = (uy | (uy << 2))  & 0x1249249249249249;

    uz = (uz | (uz << 32)) & 0x001F00000000FFFF;
    uz = (uz | (uz << 16)) & 0x001F0000FF0000FF;
    uz = (uz | (uz << 8))  & 0x100F00F00F00F00F;
    uz = (uz | (uz << 4))  & 0x10C30C30C30C30C3;
    uz = (uz | (uz << 2))  & 0x1249249249249249;

    mortonCode = ux | (uy << 1) | (uz << 2);
}

#endif
