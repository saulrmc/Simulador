#include "LinearOctree.h"
#include <iostream>
#include <cmath>
#include <algorithm>



static inline int octant_from_morton(unsigned long long mortonCode, int depth, int coordBits) {
    return (mortonCode >> (3 * (coordBits - depth))) & 0x7;
}

Vec3 LinearOctree::center_from_morton(unsigned long long mortonCode, int depth) const {
    if (depth == 0) return spatialCenter;
    int gx = 0, gy = 0, gz = 0;
    for (int d = 1; d <= depth; d++) {
        int shift = 3 * (coordBits - d);
        gx = (gx << 1) | ((mortonCode >> shift) & 1);
        gy = (gy << 1) | ((mortonCode >> (shift + 1)) & 1);
        gz = (gz << 1) | ((mortonCode >> (shift + 2)) & 1);
    }
    double cellSize = spatialSize / (1.0 * (1 << depth));
    return minPos + Vec3((gx + 0.5) * cellSize, (gy + 0.5) * cellSize, (gz + 0.5) * cellSize);
}



void LinearOctree::set_size_impl(double s) {
    spatialSize = s;
    minPos = spatialCenter - Vec3(s / 2, s / 2, s / 2);
}

void LinearOctree::set_center_impl(Vec3 c) {
    spatialCenter = c;
    if (spatialSize > 0) {
        minPos = spatialCenter - Vec3(spatialSize / 2, spatialSize / 2, spatialSize / 2);
    }
}

double LinearOctree::get_size_impl() const { return spatialSize; }
Vec3 LinearOctree::get_center_impl() const { return spatialCenter; }
double LinearOctree::get_theta_impl() const { return theta; }



void LinearOctree::create_space_impl() {
    elements.clear();
    massCenters.clear();
    levelOffset.clear();
    numLeafs = 0;
    totalNodes = 0;
    maxTreeLevel = 0;
    coordBits = 0;
    numBodies = 0;
}

void LinearOctree::refresh_theta_value_impl() {
    if (numBodies < 100) theta = 0;
    else if (numBodies < 1000) theta = 0.3;
    else if (numBodies < 10000) theta = 0.5;
    else if (numBodies < 100000) theta = 0.6;
    else if (numBodies < 1000000) theta = 0.7;
    else theta = 0.8;
}



void LinearOctree::insert_impl(const CelestialBodies& bodies) {
    numBodies = static_cast<int>(bodies.size());
    if (numBodies == 0) return;

    coordBits = 1;
    while ((1LL << coordBits) < static_cast<long long>(spatialSize) + 1) coordBits++;

    elements.clear();
    elements.reserve(numBodies);
    for (int i = 0; i < numBodies; i++) {
        elements.emplace_back(i);
        elements.back().calculate_morton_code(
            bodies.get_position(i), minPos, resolution);
    }

    std::sort(elements.begin(), elements.end(),
        [](const LinearElement& a, const LinearElement& b) {
            return a.get_morton_code() < b.get_morton_code();
        });

    numLeafs = (static_cast<unsigned int>(numBodies) + CAPACITY - 1) / CAPACITY;

    unsigned int tmp = numLeafs;
    maxTreeLevel = 0;
    while (tmp > 1) {
        tmp = (tmp + 7) / 8;
        maxTreeLevel++;
    }

    levelOffset.clear();
    levelOffset.push_back(0);

    unsigned int levelCount = numLeafs;
    unsigned int offset = numLeafs;
    for (unsigned int L = 0; L < maxTreeLevel; L++) {
        levelOffset.push_back(offset);
        levelCount = (levelCount + 7) / 8;
        offset += levelCount;
    }
    totalNodes = offset;

    massCenters.resize(totalNodes);
    for (auto& mc : massCenters) {
        mc.mass = 0;
        mc.centerOfMass = Vec3(0, 0, 0);
    }

    for (unsigned int i = 0; i < numLeafs; i++) {
        unsigned int b = i * CAPACITY;
        unsigned int e = std::min(b + CAPACITY, static_cast<unsigned int>(numBodies));
        double totalMass = 0;
        Vec3 weightedPos(0, 0, 0);
        for (unsigned int ei = b; ei < e; ei++) {
            int bodyIdx = elements[ei].get_bodyIndex();
            double m = bodies.get_mass(bodyIdx);
            totalMass += m;
            weightedPos = weightedPos + bodies.get_position(bodyIdx) * m;
        }
        massCenters[i].mass = totalMass;
        if (totalMass > 0) massCenters[i].centerOfMass = weightedPos / totalMass;
    }

    for (unsigned int L = 1; L <= maxTreeLevel; L++) {
        unsigned int prevLevelCount = (L == 1) ? numLeafs : (levelOffset[L] - levelOffset[L-1]);
        unsigned int thisLevelCount = (L < maxTreeLevel)
            ? (levelOffset[L+1] - levelOffset[L])
            : 1;

        for (unsigned int p = 0; p < thisLevelCount; p++) {
            unsigned int mcIdx = levelOffset[L] + p;
            double totalMass = 0;
            Vec3 weightedPos(0, 0, 0);
            for (int c = 0; c < 8; c++) {
                unsigned int childP = p * 8 + c;
                if (childP >= prevLevelCount) break;
                unsigned int childIdx = levelOffset[L - 1] + childP;
                if (massCenters[childIdx].mass <= 0) continue;
                totalMass += massCenters[childIdx].mass;
                weightedPos = weightedPos + massCenters[childIdx].centerOfMass
                              * massCenters[childIdx].mass;
            }
            massCenters[mcIdx].mass = totalMass;
            if (totalMass > 0) massCenters[mcIdx].centerOfMass = weightedPos / totalMass;
        }
    }
}



void LinearOctree::refresh_mass_centers_impl() {
    for (unsigned int L = 1; L <= maxTreeLevel; L++) {
        unsigned int prevLevelCount = (L == 1) ? numLeafs : (levelOffset[L] - levelOffset[L-1]);
        unsigned int thisLevelCount = (L < maxTreeLevel)
            ? (levelOffset[L+1] - levelOffset[L])
            : 1;

        for (unsigned int p = 0; p < thisLevelCount; p++) {
            unsigned int mcIdx = levelOffset[L] + p;
            double totalMass = 0;
            Vec3 weightedPos(0, 0, 0);
            for (int c = 0; c < 8; c++) {
                unsigned int childP = p * 8 + c;
                if (childP >= prevLevelCount) break;
                unsigned int childIdx = levelOffset[L - 1] + childP;
                if (massCenters[childIdx].mass <= 0) continue;
                totalMass += massCenters[childIdx].mass;
                weightedPos = weightedPos + massCenters[childIdx].centerOfMass
                              * massCenters[childIdx].mass;
            }
            massCenters[mcIdx].mass = totalMass;
            if (totalMass > 0) massCenters[mcIdx].centerOfMass = weightedPos / totalMass;
        }
    }
}



void LinearOctree::bh_traverse(unsigned int begin, unsigned int end, int depth,
                                int nodePos, CelestialBodies& bodies, int bodyIdx) {
    if (end <= begin) return;

    unsigned int count = end - begin;

    if (depth == static_cast<int>(maxTreeLevel)) {
        for (unsigned int ei = begin; ei < end; ei++) {
            int otherIdx = elements[ei].get_bodyIndex();
            if (otherIdx != bodyIdx) {
                Vec3 f = force_exerted_from_to(
                    bodies.get_mass(otherIdx),
                    bodies.get_position(otherIdx),
                    bodies.get_mass(bodyIdx),
                    bodies.get_position(bodyIdx));
                bodies.set_force(bodies.get_force(bodyIdx) + f, bodyIdx);
            }
        }
        return;
    }

    unsigned int treeLevel = maxTreeLevel - depth;
    unsigned int mcIdx = levelOffset[treeLevel] + nodePos;
    const MassCenter& mc = massCenters[mcIdx];

    if (mc.mass <= 0) return;

    double dx = bodies.get_posX(bodyIdx) - mc.centerOfMass.get_x();
    double dy = bodies.get_posY(bodyIdx) - mc.centerOfMass.get_y();
    double dz = bodies.get_posZ(bodyIdx) - mc.centerOfMass.get_z();
    double d2 = dx * dx + dy * dy + dz * dz;

    if (d2 == 0) return;

    double nodeSize = spatialSize / (1.0 * (1 << depth));

    if (nodeSize * nodeSize < d2 * theta * theta) {
        Vec3 f = force_exerted_from_to(
            mc.mass, mc.centerOfMass,
            bodies.get_mass(bodyIdx), bodies.get_position(bodyIdx));
        bodies.set_force(bodies.get_force(bodyIdx) + f, bodyIdx);
    }
    else if (count <= CAPACITY) {
        for (unsigned int ei = begin; ei < end; ei++) {
            int otherIdx = elements[ei].get_bodyIndex();
            if (otherIdx != bodyIdx) {
                Vec3 f = force_exerted_from_to(
                    bodies.get_mass(otherIdx),
                    bodies.get_position(otherIdx),
                    bodies.get_mass(bodyIdx),
                    bodies.get_position(bodyIdx));
                bodies.set_force(bodies.get_force(bodyIdx) + f, bodyIdx);
            }
        }
    }
    else {
        int nextDepth = depth + 1;
        int childNodePos = nodePos * 8;
        unsigned int childBegin = begin;
        int prevOctant = octant_from_morton(elements[begin].get_morton_code(), nextDepth, coordBits);

        for (unsigned int i = begin + 1; i <= end; i++) {
            int curOctant = (i < end)
                ? octant_from_morton(elements[i].get_morton_code(), nextDepth, coordBits)
                : -1;
            if (curOctant != prevOctant) {
                bh_traverse(childBegin, i, nextDepth, childNodePos, bodies, bodyIdx);
                childNodePos++;
                childBegin = i;
                prevOctant = curOctant;
            }
        }
    }
}

void LinearOctree::calc_forces_impl(CelestialBodies& bodies) {
    if (totalNodes == 0 || massCenters.empty()) return;

    for (int i = 0; i < numBodies; i++) {
        bodies.set_force(Vec3(0, 0, 0), i);
    }

    unsigned int rootMcIdx = totalNodes - 1;
    if (massCenters[rootMcIdx].mass <= 0) return;

    for (int bodyIdx = 0; bodyIdx < numBodies; bodyIdx++) {
        bh_traverse(0, static_cast<unsigned int>(numBodies), 0, 0, bodies, bodyIdx);
    }
}
