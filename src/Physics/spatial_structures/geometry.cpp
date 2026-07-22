//
// Created by Saúl on 15/02/2026.
//

#include "geometry.h"
#include "CelestialBodies.h"

Vec3 closest_point(const Vec3& nodeCenter, double nodeSize, const Vec3& bodyCenter) {
    Vec3 closestPoint;
    if (nodeCenter.get_x() - nodeSize/2 < bodyCenter.get_x() and
        nodeCenter.get_x() + nodeSize/2 > bodyCenter.get_x())
        closestPoint.set_x(bodyCenter.get_x());
    else if (nodeCenter.get_x() < bodyCenter.get_x())
        closestPoint.set_x(nodeCenter.get_x() + nodeSize/2);
    else closestPoint.set_x(nodeCenter.get_x() - nodeSize/2);

    if (nodeCenter.get_y() - nodeSize/2 < bodyCenter.get_y() and
        nodeCenter.get_y() + nodeSize/2 > bodyCenter.get_y())
        closestPoint.set_y(bodyCenter.get_y());
    else if (nodeCenter.get_y() < bodyCenter.get_y())
        closestPoint.set_y(nodeCenter.get_y() + nodeSize/2);
    else closestPoint.set_y(nodeCenter.get_y() - nodeSize/2);

    if (nodeCenter.get_z() - nodeSize/2 < bodyCenter.get_z() and
            nodeCenter.get_z() + nodeSize/2 > bodyCenter.get_z())
        closestPoint.set_z(bodyCenter.get_z());
    else if (nodeCenter.get_z() < bodyCenter.get_z())
        closestPoint.set_z(nodeCenter.get_z() + nodeSize/2);
    else closestPoint.set_z(nodeCenter.get_z() - nodeSize/2);

    return closestPoint;
}

bool overlap_node(const Vec3& nodeCenter, double nodeSize, CelestialBodies& bodies, int bodyIndex) {
    if (bodyIndex < 0 || bodyIndex >= (int)bodies.size()) return false;
    if (bodies.is_deleted(bodyIndex)) return false;
    Vec3 bodyPosition = bodies.get_position(bodyIndex);
    double bodyRadius = bodies.get_radius(bodyIndex);
    Vec3 closestPt = closest_point(nodeCenter, nodeSize, bodyPosition);
    double distance = closestPt.distance(bodyPosition);
    return distance <= bodyRadius;
}

bool overlap_body(const Vec3& center1, const Vec3& center2, double radius1, double radius2) {
    double distance = (center2 - center1).magnitude();
    return radius1 + radius2 - distance >= 0;
}
