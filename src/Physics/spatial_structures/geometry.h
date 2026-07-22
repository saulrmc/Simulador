//
// Created by Saúl on 15/02/2026.
//

#pragma once
#ifndef SIMULADORGRAVITACIONAL_GEOMETRY_H
#define SIMULADORGRAVITACIONAL_GEOMETRY_H
#include "Vec3.h"

class CelestialBodies;

Vec3 closest_point(const Vec3& nodeCenter, double nodeSize, const Vec3& bodyCenter);
bool overlap_node(const Vec3& nodeCenter, double nodeSize, CelestialBodies& bodies, int bodyIndex);
bool overlap_body(const Vec3& center1, const Vec3& center2, double radius1, double radius2);

#endif //SIMULADORGRAVITACIONAL_GEOMETRY_H
