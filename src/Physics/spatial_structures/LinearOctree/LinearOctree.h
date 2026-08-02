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

//nodo del subárbol virtual (profundidad > treeDepth) de una hoja saturada:
//la subdivisión se precomputa una vez por refresh en un arreglo compacto por
//hoja (en orden por niveles: los hijos directos de un nodo son un rango
//contiguo en el siguiente nivel, [childIdx, childIdx+popcount(childMask))),
//para que calc_subgroup recorra los subgrupos sin re-sumar rangos ni acceder
//a arreglos globales dispersos (el cuello de botella del disco)
struct VirtualNode {
    double mass;
    Vec3 com;
    double maxR;
    uint32_t start;
    uint32_t count;
    uint32_t childIdx;
    uint8_t childMask;
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
    void calc_forces_impl_recursive(CelestialBodies& bodies, int bodyIndex, int nodeIndex, int depth);
    void calc_subgroup(CelestialBodies& bodies, int bodyIndex,
        uint64_t a, uint64_t b, int depth, double cellSize, double mass, Vec3 com,
        const std::vector<VirtualNode>& vnodes, uint32_t vnode);
    void build_virtual_octrees(CelestialBodies& bodies);
    void build_virtual_tree(CelestialBodies& bodies, std::vector<VirtualNode>& vec,
        uint64_t a, uint64_t b, int startDepth);
    Vec3 cell_center_of(int nodeIndex, int depth) const;
    void refresh_mass_centers_impl(CelestialBodies& bodies);
    void refresh_theta_value_impl();
    void set_size_impl(int size);
    void set_center_impl(Vec3 center);
    double get_size_impl() const;
    Vec3 get_center_impl() const;
    double get_theta_impl() const;
    void compute_mass_centers_bottom_up();

    template<typename Callback>
    void query_region_impl(Callback&& on_pair, CelestialBodies& bodies, int bodyIndex) {
        if (numBodies <= 0 || elements.empty() || massCenters.empty() || maxRadius.empty()) return;
        recursive_query_region(0, 0, std::forward<Callback>(on_pair), bodies, bodyIndex);
    }

    template<typename Callback>
    void recursive_query_region(int nodeIndex, int depth, Callback&& on_pair,
        CelestialBodies& bodies, int bodyIndex) {
        if (massCenters[nodeIndex].mass == 0) return;
        if (bodies.is_deleted(bodyIndex)) return;
        //poda: la esfera del cuerpo expandida con el radio máximo de la celda
        //garantiza que ninguna superposición real se descarte (la esfera de un
        //cuerpo cercano puede extenderse más allá del radio del consultado)
        Vec3 cellCenter = cell_center_of(nodeIndex, depth);
        double cellHalf = (double)(spatialSize >> depth) / 2.0;
        double px = bodies.get_posX(bodyIndex), py = bodies.get_posY(bodyIndex), pz = bodies.get_posZ(bodyIndex);
        double radius = bodies.get_radius(bodyIndex) + maxRadius[nodeIndex];
        double cx = std::min(std::max(px, cellCenter.get_x() - cellHalf), cellCenter.get_x() + cellHalf);
        double cy = std::min(std::max(py, cellCenter.get_y() - cellHalf), cellCenter.get_y() + cellHalf);
        double cz = std::min(std::max(pz, cellCenter.get_z() - cellHalf), cellCenter.get_z() + cellHalf);
        double d2 = (cx - px) * (cx - px) + (cy - py) * (cy - py) + (cz - pz) * (cz - pz);
        if (d2 > radius * radius) return;
        if (depth == (int)treeDepth) {
            uint64_t leafIdx = (uint64_t)nodeIndex - leafOffset;
            const std::vector<VirtualNode>& vnodes = leafVirtual[(size_t)leafIdx];
            if (!vnodes.empty()) {
                //hoja saturada: los cuerpos quedaron en una celda degenerada del
                //disco y no se pueden subdividir más en el octree físico; se
                //desciende el subárbol virtual (con poda por celda y filtro por
                //distancia real) para no revisar ~143 candidatos por consulta
                Vec3 cellCenter = cell_center_of(nodeIndex, depth);
                double cellHalf = (double)(spatialSize >> depth) / 2.0;
                double radius = bodies.get_radius(bodyIndex) + vnodes[0].maxR;
                recursive_query_virtual(vnodes, 0, std::forward<Callback>(on_pair),
                    bodies, bodyIndex, cellCenter, cellHalf, radius);
                return;
            }
            uint64_t a = leafStart[leafIdx], b = leafStart[leafIdx + 1];
            for (uint64_t k = a; k < b; k++) {
                int otherIndex = elements[k].get_body_index();
                if (otherIndex != bodyIndex && bodyIndex < otherIndex)
                    on_pair(bodies, bodyIndex, otherIndex);
            }
        } else {
            for (int c = 0; c < 8; c++)
                recursive_query_region(8 * nodeIndex + 1 + c, depth + 1,
                    std::forward<Callback>(on_pair), bodies, bodyIndex);
        }
    }

    template<typename Callback>
    void recursive_query_virtual(const std::vector<VirtualNode>& vnodes, uint32_t vnode,
        Callback&& on_pair, CelestialBodies& bodies, int bodyIndex,
        const Vec3& cellCenter, double cellHalf, double radius) {
        const VirtualNode& n = vnodes[vnode];
        const double px = bodies.get_posX(bodyIndex);
        const double py = bodies.get_posY(bodyIndex);
        const double pz = bodies.get_posZ(bodyIndex);
        const double cx = std::min(std::max(px, cellCenter.get_x() - cellHalf), cellCenter.get_x() + cellHalf);
        const double cy = std::min(std::max(py, cellCenter.get_y() - cellHalf), cellCenter.get_y() + cellHalf);
        const double cz = std::min(std::max(pz, cellCenter.get_z() - cellHalf), cellCenter.get_z() + cellHalf);
        const double d2 = (cx - px) * (cx - px) + (cy - py) * (cy - py) + (cz - pz) * (cz - pz);
        if (d2 > radius * radius) return;
        if (n.childMask == 0) {
            //nodo virtual hoja: revisar solo los cuerpos que de verdad superponen
            //la esfera del consultado (el radio real de cada candidato es mucho
            //más chico que la celda degenerada)
            for (uint64_t k = n.start; k < n.start + n.count; k++) {
                int otherIndex = elements[k].get_body_index();
                if (otherIndex == bodyIndex || bodyIndex > otherIndex) continue;
                const double ox = bodies.get_posX(otherIndex);
                const double oy = bodies.get_posY(otherIndex);
                const double oz = bodies.get_posZ(otherIndex);
                const double ddx = ox - px, ddy = oy - py, ddz = oz - pz;
                const double sumR = bodies.get_radius(bodyIndex) + bodies.get_radius(otherIndex);
                if (ddx * ddx + ddy * ddy + ddz * ddz <= sumR * sumR)
                    on_pair(bodies, bodyIndex, otherIndex);
            }
            return;
        }
        const double half2 = cellHalf * 0.5;
        for (int c = 0; c < 8; c++) {
            if (((n.childMask >> c) & 1) == 0) continue;
            uint32_t childIdx = n.childIdx +
                (uint32_t)__builtin_popcount((unsigned)(n.childMask & ((uint8_t)(1u << c) - 1)));
            const VirtualNode& child = vnodes[childIdx];
            Vec3 childCenter(
                cellCenter.get_x() + (((c & 1) != 0) ? half2 : -half2),
                cellCenter.get_y() + (((c & 2) != 0) ? half2 : -half2),
                cellCenter.get_z() + (((c & 4) != 0) ? half2 : -half2));
            recursive_query_virtual(vnodes, childIdx, std::forward<Callback>(on_pair),
                bodies, bodyIndex, childCenter, half2,
                bodies.get_radius(bodyIndex) + child.maxR);
        }
    }

    std::vector<LinearElement> elements;
    std::vector<MassCenter> massCenters;
    std::vector<uint32_t> leafStart;
    std::vector<double> maxRadius;
    //subárboles virtuales (profundidad > treeDepth) de las hojas saturadas,
    //precomputados una vez por refresh; permiten a calc_subgroup recorrer los
    //subgrupos con acceso contiguo en caché (en el disco degenerado todas las
    //hojas están saturadas y esto domina el tiempo de cálculo)
    std::vector<std::vector<VirtualNode>> leafVirtual;
    int numBodies;
    double theta;
    int spatialSize;
    Vec3 spatialCenter;
    Vec3 minPos;
    uint8_t resolutionLevel; //max 21
    uint8_t treeDepth;
    uint64_t leafOffset;
};

inline LinearOctree::LinearOctree() {
    resolutionLevel = 10;
    numBodies = 0;
    theta = 0;
    spatialSize = 0;
    treeDepth = 0;
    leafOffset = 0;
    name = "LinearOctree";
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

inline void LinearOctree::set_size_impl(int size) {
    this->spatialSize = size;
    this->minPos = spatialCenter - Vec3(spatialSize / 2.0, spatialSize / 2.0, spatialSize / 2.0);
}
inline void LinearOctree::set_center_impl(Vec3 center) {
    this->spatialCenter = center;
    this->minPos = spatialCenter - Vec3(spatialSize / 2.0, spatialSize / 2.0, spatialSize / 2.0);
}
inline double LinearOctree::get_size_impl() const {return this->spatialSize;}
inline Vec3 LinearOctree::get_center_impl() const {return this->spatialCenter;}
inline double LinearOctree::get_theta_impl() const {return this->theta;}

#endif
