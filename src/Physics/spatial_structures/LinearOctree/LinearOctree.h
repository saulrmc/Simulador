#pragma once
#ifndef SIMULADORGRAVITACIONAL_LINEAROCTREE_H
#define SIMULADORGRAVITACIONAL_LINEAROCTREE_H

#include <vector>
#include <algorithm>
#include <cstdint>
#include <cmath>

#include "LinearElement.h"
#include "../SpatialStructure.h"
#include "../geometry.h"
#include "../../Physics.h"
#include "CelestialBodies.h"

struct MassCenter {
    double mass;
    Vec3 centerOfMass;
};

class LinearOctree : public SpatialStructure<LinearOctree> {
    friend class SpatialStructure<LinearOctree>;
public:
    LinearOctree();
    ~LinearOctree();

private:
    void create_space_impl();
    void insert_impl(const CelestialBodies& bodies);
    void calc_forces_impl(CelestialBodies& bodies);
    void refresh_mass_centers_impl();
    void refresh_theta_value_impl();
    void set_size_impl(double s);
    void set_center_impl(Vec3 c);
    double get_size_impl() const;
    Vec3 get_center_impl() const;
    double get_theta_impl() const;

    template<typename Callback>
    void query_region_impl(Callback&& on_pair, CelestialBodies& bodies, int bodyIndex) {
        double leafSize = spatialSize / (1.0 * (1 << maxTreeLevel));
        for (unsigned int i = 0; i < numLeafs; i++) {
            unsigned int b = i * CAPACITY;
            unsigned int e = std::min(b + CAPACITY, static_cast<unsigned int>(numBodies));
            if (b >= elements.size()) break;
            Vec3 leafCenter = center_from_morton(elements[b].get_morton_code(), maxTreeLevel);
            if (!overlap_node(leafCenter, leafSize, bodies, bodyIndex)) continue;
            for (unsigned int ei = b; ei < e; ei++) {
                int otherIndex = elements[ei].get_bodyIndex();
                if (otherIndex != bodyIndex && bodyIndex < otherIndex) {
                    on_pair(bodies, bodyIndex, otherIndex);
                }
            }
        }
    }

    void bh_traverse(unsigned int begin, unsigned int end, int depth,
                     int nodePos, CelestialBodies& bodies, int bodyIdx);

    Vec3 center_from_morton(unsigned long long mortonCode, int depth) const;

    std::vector<LinearElement> elements;
    std::vector<MassCenter> massCenters;
    std::vector<unsigned int> levelOffset;

    unsigned int numLeafs;
    unsigned int totalNodes;
    unsigned int maxTreeLevel;
    int coordBits;
    int numBodies;
    double theta;
    double spatialSize;
    Vec3 spatialCenter;
    Vec3 minPos;
    double resolution;
};

inline LinearOctree::LinearOctree()
    : numLeafs(0), totalNodes(0), maxTreeLevel(0), coordBits(0),
      numBodies(0), theta(0), spatialSize(0),
      spatialCenter(Vec3(0, 0, 0)), minPos(Vec3(0, 0, 0)), resolution(1.0) {}

inline LinearOctree::~LinearOctree() {}

#endif
