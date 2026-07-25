#pragma once
#ifndef SIMULADORGRAVITACIONAL_LINEAROCTREE_H
#define SIMULADORGRAVITACIONAL_LINEAROCTREE_H

#include <vector>
#include <algorithm>
#include <cstdint>

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
    void reorder_data(CelestialBodies & bodies);
    void insert_impl(CelestialBodies& bodies);
    void calc_forces_impl(CelestialBodies& bodies);
    void refresh_mass_centers_impl();
    void refresh_theta_value_impl();
    void set_size_impl(double size);
    void set_center_impl(Vec3 center);
    double get_size_impl() const;
    Vec3 get_center_impl() const;
    double get_theta_impl() const;
    void compute_mass_centers_bottom_up();

    template<typename Callback>
    void query_region_impl(Callback&& on_pair, CelestialBodies& bodies, int bodyIndex) ;

    Vec3 center_from_morton(unsigned long long mortonCode, int depth) const;

    std::vector<LinearElement> elements;
    std::vector<MassCenter> massCenters;
    bool needToReorder;
    int numBodies;
    double theta;
    double spatialSize;
    Vec3 spatialCenter;
    Vec3 minPos;
    uint8_t resolutionLevel; //max 21
};

inline LinearOctree::LinearOctree() {
    resolutionLevel = 10;
    numBodies = 0;
    theta = 0;
    spatialSize = 0;
    needToReorder = true;
}

inline LinearOctree::~LinearOctree() = default;

inline void LinearOctree::refresh_theta_value_impl() {
    if (numBodies < 100) this->theta = 0;
    else if (numBodies < 1000) this->theta = 0.3;
    else if (numBodies < 10000) this->theta = 0.5;
    else if (numBodies < 100000) this->theta = 0.6;
    else if (numBodies < 1000000) this->theta = 0.7;
    else theta = 0.8;
}

inline void LinearOctree::set_size_impl(double size) {this->spatialSize = size;}
inline void LinearOctree::set_center_impl(Vec3 center) {this->spatialCenter = center;}
inline double LinearOctree::get_size_impl() const {return this->spatialSize;}
inline Vec3 LinearOctree::get_center_impl() const {return this->spatialCenter;}
inline double LinearOctree::get_theta_impl() const {return this->theta;}

#endif
