//
// Created by saul on 10/06/26.
//

#pragma once
#ifndef SIMULADORGRAVITACIONAL_SPATIALSTRUCTURE_H
#define SIMULADORGRAVITACIONAL_SPATIALSTRUCTURE_H
#include "Vec3.h"
#include "CelestialBodies.h"

constexpr int CAPACITY = 4;

template <typename Derived>
class SpatialStructure {
public:
    void create_space() { static_cast<Derived*>(this)->create_space_impl(); }
    void insert(const CelestialBodies& bodies) { static_cast<Derived*>(this)->insert_impl(bodies); }
    void calc_forces(CelestialBodies& bodies) { static_cast<Derived*>(this)->calc_forces_impl(bodies); }
    void refresh_mass_centers() { static_cast<Derived*>(this)->refresh_mass_centers_impl(); }
    void refresh_theta_value() { static_cast<Derived*>(this)->refresh_theta_value_impl(); }
    void set_size(double s) { static_cast<Derived*>(this)->set_size_impl(s); }
    void set_center(Vec3 c) { static_cast<Derived*>(this)->set_center_impl(c); }
    double get_size() const { return static_cast<const Derived*>(this)->get_size_impl(); }
    Vec3 get_center() const { return static_cast<const Derived*>(this)->get_center_impl(); }
    double get_theta() const { return static_cast<const Derived*>(this)->get_theta_impl(); }

    template<typename Callback>
    void query_region(Callback&& on_pair, CelestialBodies& bodies, int bodyIndex) {
        static_cast<Derived*>(this)->query_region_impl(
            std::forward<Callback>(on_pair), bodies, bodyIndex);
    }
};

#endif //SIMULADORGRAVITACIONAL_SPATIALSTRUCTURE_H
