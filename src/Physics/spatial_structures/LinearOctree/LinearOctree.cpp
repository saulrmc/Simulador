//
// Created by saul on 23/07/26.
//

#include "LinearOctree.h"

void LinearOctree::create_space_impl() {
    numBodies = 0;
    treeDepth = 0;
    leafOffset = 0;
    elements.clear();
    massCenters.clear();
    leafStart.clear();
    maxRadius.clear();
    leafVirtual.clear();
    minPos = spatialCenter - Vec3(spatialSize / 2.0, spatialSize / 2.0, spatialSize / 2.0);
}

void LinearOctree::reorder_data(CelestialBodies &bodies) {
    //Permutación en el lugar: el elemento i debe terminar con el cuerpo A[σ(i)],
    //donde σ(i) = elements[i].bodyIndex. Se recorren los ciclos de σ y se rota
    //los cuerpos hacia atrás a lo largo de cada ciclo con swaps adyacentes.
    size_t n = elements.size();
    for (size_t i = 0; i < n; ++i) {
        if (elements[i].get_body_index() == i) continue;
        std::vector<size_t> cycle;
        cycle.push_back(i);
        size_t cur = elements[i].get_body_index();
        while (cur != i) {
            cycle.push_back(cur);
            cur = elements[cur].get_body_index();
        }
        for (size_t k = 0; k + 1 < cycle.size(); ++k)
            bodies.swap(cycle[k], cycle[k + 1]);
        for (size_t c : cycle) elements[c].bodyIndex = c;
    }
}

void LinearOctree::insert_impl(CelestialBodies &bodies) {
    numBodies = bodies.size();
    elements.clear();
    elements.reserve(numBodies);
    //generar códigos de Morton
    for (int i = 0; i < numBodies; i++) {
        LinearElement element;
        element.calculate_morton_code(bodies.get_position(i), minPos, resolutionLevel, spatialSize);
        element.set_body_index(i);
        elements.push_back(element);
    }
    //ordenar
    std::sort(elements.begin(), elements.end(),
        [](const LinearElement& element1, const LinearElement& element2) {
            return element1.get_morton_code() < element2.get_morton_code();
        });
    //dejar el SoA ordenado de tal forma que el cuerpo i quede en la posición i
    reorder_data(bodies);

    //las inserciones son por lote, por lo que la profundidad del árbol solo depende
    //de la cantidad de cuerpos actual
    uint64_t leavesNeeded = ((uint64_t)numBodies + CAPACITY - 1) / CAPACITY;
    treeDepth = 0;
    uint64_t numLeaves = 1;
    while (numLeaves < leavesNeeded && treeDepth < resolutionLevel) {
        numLeaves *= 8;
        treeDepth++;
    }
    leafOffset = (numLeaves - 1) / 7;
}

void LinearOctree::refresh_mass_centers_impl(CelestialBodies &bodies) {
    uint64_t numLeaves = uint64_t(1) << (3 * treeDepth);
    uint64_t totalNodes = leafOffset + numLeaves;

    massCenters.assign(totalNodes, MassCenter{0, Vec3(0, 0, 0)});
    maxRadius.assign(totalNodes, 0.0);
    leafStart.assign(numLeaves + 1, 0);

    //sweep O(n): contar cuántos cuerpos caen en cada hoja
    int shift = 3 * (resolutionLevel - treeDepth);
    for (int i = 0; i < numBodies; i++) {
        leafStart[elements[i].get_morton_code() >> shift]++;
    }
    //prefijo acumulado: leafStart[l] = inicio del rango de cuerpos de la hoja l
    uint32_t running = 0;
    for (uint64_t l = 0; l < numLeaves; l++) {
        uint32_t count = leafStart[l];
        leafStart[l] = running;
        running += count;
    }
    leafStart[numLeaves] = running;

    //llenar las hojas con los rangos contiguos de cuerpos reordenados
    for (int i = 0; i < numBodies; i++) {
        uint32_t leafIdx = elements[i].get_morton_code() >> shift;
        MassCenter& mc = massCenters[leafOffset + leafIdx];
        double m = bodies.get_mass(i);
        mc.mass += m;
        mc.centerOfMass = mc.centerOfMass + bodies.get_position(i) * m;
        double r = bodies.get_radius(i);
        if (r > maxRadius[leafOffset + leafIdx]) maxRadius[leafOffset + leafIdx] = r;
    }
    for (uint64_t l = 0; l < numLeaves; l++) {
        MassCenter& mc = massCenters[leafOffset + l];
        if (mc.mass > 0) mc.centerOfMass = mc.centerOfMass / mc.mass;
    }

    compute_mass_centers_bottom_up();
    build_virtual_octrees(bodies);
}

void LinearOctree::build_virtual_octrees(CelestialBodies &bodies) {
    uint64_t numLeaves = uint64_t(1) << (3 * treeDepth);
    leafVirtual.assign((size_t)numLeaves, std::vector<VirtualNode>());
    for (uint64_t l = 0; l < numLeaves; l++) {
        uint64_t a = leafStart[l], b = leafStart[l + 1];
        if (b - a > (uint64_t)CAPACITY)
            build_virtual_tree(bodies, leafVirtual[(size_t)l], a, b, (int)treeDepth);
    }
}

void LinearOctree::build_virtual_tree(CelestialBodies &bodies,
    std::vector<VirtualNode>& nodes, uint64_t a, uint64_t b, int startDepth) {
    struct Item {
        uint64_t start, count;
        int depth;
        size_t nodeIdx;
    };
    std::vector<Item> cur, next;
    VirtualNode root;
    root.mass = 0;
    root.com = Vec3(0, 0, 0);
    root.maxR = 0;
    root.start = (uint32_t)a;
    root.count = (uint32_t)(b - a);
    root.childIdx = 0;
    root.childMask = 0;
    nodes.push_back(root);
    cur.push_back({a, b - a, startDepth, 0});
    while (!cur.empty()) {
        size_t nextStart = nodes.size();
        size_t acc = 0;
        next.clear();
        for (const Item& it : cur) {
            double m = 0;
            Vec3 c(0, 0, 0);
            double mr = 0;
            for (uint64_t j = it.start; j < it.start + it.count; j++) {
                double mi = bodies.get_mass(j);
                m += mi;
                c = c + bodies.get_position(j) * mi;
                double ri = bodies.get_radius(j);
                if (ri > mr) mr = ri;
            }
            c = c / m;
            nodes[it.nodeIdx].mass = m;
            nodes[it.nodeIdx].com = c;
            nodes[it.nodeIdx].maxR = mr;
            if (it.count <= (uint64_t)CAPACITY || it.depth >= (int)resolutionLevel) continue;
            //los hijos directos de todos los nodos del nivel van contiguos en
            //el siguiente nivel; childIdx apunta al primero de ellos. No se
            //guardan referencias a nodes a través del push_back (puede realocar)
            uint32_t childIdxVal = (uint32_t)(nextStart + acc);
            uint8_t mask = 0;
            int shift = 3 * (resolutionLevel - it.depth - 1);
            uint64_t end = it.start + it.count;
            uint64_t k = it.start;
            while (k < end) {
                uint64_t octant = (elements[k].get_morton_code() >> shift) & 7ULL;
                uint64_t k2 = k + 1;
                while (k2 < end && ((elements[k2].get_morton_code() >> shift) & 7ULL) == octant) k2++;
                VirtualNode ch;
                ch.mass = 0;
                ch.com = Vec3(0, 0, 0);
                ch.maxR = 0;
                ch.start = (uint32_t)k;
                ch.count = (uint32_t)(k2 - k);
                ch.childIdx = 0;
                ch.childMask = 0;
                nodes.push_back(ch);
                next.push_back({k, k2 - k, it.depth + 1, nodes.size() - 1});
                mask = (uint8_t)(mask | (uint8_t)(1u << octant));
                acc++;
                k = k2;
            }
            nodes[it.nodeIdx].childIdx = childIdxVal;
            nodes[it.nodeIdx].childMask = mask;
        }
        cur.swap(next);
    }
}

void LinearOctree::compute_mass_centers_bottom_up() {
    //recorrido inverso por índices: los hijos (8i+1..8i+8) siempre tienen
    //índices mayores que el padre, así que un solo pase en orden descendente basta
    for (uint64_t i = leafOffset; i-- > 0;) {
        MassCenter& parent = massCenters[i];
        for (int c = 0; c < 8; c++) {
            const MassCenter& child = massCenters[8 * i + 1 + c];
            if (child.mass > 0) {
                double newMass = parent.mass + child.mass;
                parent.centerOfMass = (parent.centerOfMass * parent.mass
                    + child.centerOfMass * child.mass) / newMass;
                parent.mass = newMass;
            }
            double childMax = maxRadius[8 * i + 1 + c];
            if (childMax > maxRadius[i]) maxRadius[i] = childMax;
        }
    }
}

void LinearOctree::calc_forces_impl(CelestialBodies &bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        bodies.set_force(Vec3(0, 0, 0), i);
        calc_forces_impl_recursive(bodies, i, 0, 0);
    }
}

void LinearOctree::calc_subgroup(CelestialBodies &bodies, int bodyIndex,
    uint64_t a, uint64_t b, int depth, double cellSize, double mass, Vec3 com,
    const std::vector<VirtualNode>& vnodes, uint32_t vnode) {
    if (a >= b) return;
    if (b - a <= (uint64_t)CAPACITY || depth >= (int)resolutionLevel) {
        for (uint64_t k = a; k < b; k++) {
            if (k == (uint64_t)bodyIndex) continue;
            Vec3 new_force = force_exerted_from_to(
                bodies.get_mass(k),
                bodies.get_position(k),
                bodies.get_mass(bodyIndex),
                bodies.get_position(bodyIndex)
            );
            bodies.set_force(bodies.get_force(bodyIndex) + new_force, bodyIndex);
        }
        return;
    }
    const double d_x = bodies.get_posX(bodyIndex) - com.get_x();
    const double d_y = bodies.get_posY(bodyIndex) - com.get_y();
    const double d_z = bodies.get_posZ(bodyIndex) - com.get_z();
    const double d_2 = d_x * d_x + d_y * d_y + d_z * d_z;
    //el subgrupo que contiene al cuerpo consultado nunca se agrega como monopolo
    //(fuerza-bruta local); los demás se agregan si satisfacen el criterio θ
    const bool containsBody = (uint64_t)bodyIndex >= a && (uint64_t)bodyIndex < b;
    if (!containsBody && d_2 != 0 && cellSize * cellSize < d_2 * theta * theta) {
        Vec3 new_force = force_exerted_from_to(
            mass,
            com,
            bodies.get_mass(bodyIndex),
            bodies.get_position(bodyIndex)
        );
        bodies.set_force(bodies.get_force(bodyIndex) + new_force, bodyIndex);
        return;
    }
    //los subgrupos de la siguiente profundidad ya están precomputados (una vez
    //por refresh) en el arreglo compacto de la hoja: recorrido contiguo en caché
    const VirtualNode& node = vnodes[vnode];
    for (int c = 0; c < 8; c++) {
        if (((node.childMask >> c) & 1) == 0) continue;
        uint32_t childIdx = node.childIdx +
            (uint32_t)__builtin_popcount((unsigned)(node.childMask & ((uint8_t)(1u << c) - 1)));
        const VirtualNode& child = vnodes[childIdx];
        calc_subgroup(bodies, bodyIndex,
            (uint64_t)child.start, (uint64_t)child.start + child.count, depth + 1,
            cellSize * 0.5, child.mass, child.com, vnodes, childIdx);
    }
}

Vec3 LinearOctree::cell_center_of(int nodeIndex, int depth) const {
    Vec3 center = spatialCenter;
    uint64_t k = (uint64_t)nodeIndex - ((1ULL << (3 * depth)) - 1) / 7;
    for (int level = 0; level < depth; level++) {
        int octant = (int)((k >> (3 * (depth - 1 - level))) & 7ULL);
        double off = (double)(spatialSize >> (level + 1)) / 2.0;
        double sx = center.get_x(), sy = center.get_y(), sz = center.get_z();
        center.set_x(sx + ((octant & 1) ? off : -off));
        center.set_y(sy + ((octant & 2) ? off : -off));
        center.set_z(sz + ((octant & 4) ? off : -off));
    }
    return center;
}

void LinearOctree::calc_forces_impl_recursive(CelestialBodies &bodies, int bodyIndex,
    int nodeIndex, int depth) {
    const MassCenter& mc = massCenters[nodeIndex];
    if (mc.mass == 0) return;

    const double d_x = bodies.get_posX(bodyIndex) - mc.centerOfMass.get_x();
    const double d_y = bodies.get_posY(bodyIndex) - mc.centerOfMass.get_y();
    const double d_z = bodies.get_posZ(bodyIndex) - mc.centerOfMass.get_z();
    const double d_2 = d_x * d_x + d_y * d_y + d_z * d_z;

    if (d_2 == 0) return;
    int nodeSize = spatialSize >> depth;
    if (nodeSize * nodeSize < d_2 * theta * theta) {
        Vec3 new_force = force_exerted_from_to(
            mc.mass,
            mc.centerOfMass,
            bodies.get_mass(bodyIndex),
            bodies.get_position(bodyIndex)
        );
        bodies.set_force(bodies.get_force(bodyIndex) + new_force, bodyIndex);
    }
    else if (depth == (int)treeDepth) {
        //hoja: los cuerpos que contiene son un rango contiguo de índices;
        //si la hoja está saturada se subdivide con el subárbol virtual
        //precomputado (profundidad adaptativa a la densidad local)
        uint64_t leafIdx = nodeIndex - leafOffset;
        const std::vector<VirtualNode>& vnodes = leafVirtual[(size_t)leafIdx];
        calc_subgroup(bodies, bodyIndex,
            leafStart[leafIdx], leafStart[leafIdx + 1], depth, (double)nodeSize,
            mc.mass, mc.centerOfMass, vnodes, 0);
    }
    else {
        for (int c = 0; c < 8; c++)
            calc_forces_impl_recursive(bodies, bodyIndex, 8 * nodeIndex + 1 + c, depth + 1);
    }
}

