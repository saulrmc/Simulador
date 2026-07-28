//
// Created by saul on 23/07/26.
//

#include "LinearOctree.h"

void calc_forces_impl(CelestialBodies &bodies) {

}

void LinearOctree::create_space_impl() {

}

void LinearOctree::reorder_data(CelestialBodies &bodies) {
    //Cycle Sorting
    size_t n = elements.size();
    for (size_t i = 0; i < n; ++i) {
        // Mientras el objeto que está en 'elements[i].bodyIndex'
        // no sea el que realmente pertenece a la posición 'i'
        while (elements[i].get_body_index() != i) {
            size_t targetIdx = elements[i].get_body_index();

            // Intercambiamos los objetos pesados en el lugar (O(1) memoria)
            bodies.swap(i, targetIdx);

            // Intercambiamos también los índices en el octree para mantener el rastro
            std::swap(elements[i].bodyIndex, elements[targetIdx].bodyIndex);
        }
    }
}

void LinearOctree::insert_impl(CelestialBodies &bodies) {
    elements.reserve(bodies.size());
    //generar códigos de Morton
    for (int i = 0; i < bodies.size(); i++) {
        LinearElement element;
        element.calculate_morton_code(bodies.get_position(i), minPos, resolutionLevel, spatialSize);
        element.set_body_index(i);
        elements.push_back(element);
        numBodies++;
    }
    //ordenar
    std::sort(elements.begin(), elements.end(),
        [](LinearElement element1, LinearElement element2) {
        return element1.get_morton_code() < element2.get_morton_code();
    });
    //reordenar si es la primera vez
    if (needToReorder) {
        needToReorder = false;
        reorder_data(bodies);
    }
}

void LinearOctree::calc_forces_impl(CelestialBodies &bodies) {

}

void LinearOctree::refresh_mass_centers_impl() {

}

void LinearOctree::compute_mass_centers_bottom_up() {

}
