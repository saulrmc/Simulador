//
// Created by Saúl on 21/12/2025.
//

#include "CalculationManager.h"

CalculationManager::CalculationManager() {
    root = nullptr;
}

CalculationManager::~CalculationManager() {
    delete root;
}

void CalculationManager::step() {

}

void CalculationManager::create_Octree() {
    root->create_space();
}

void CalculationManager::reinsert_nodes(std::vector<CelestialBody> &bodies) {
    for (CelestialBody body : bodies) {
        root->insert(&body);
    }
}


