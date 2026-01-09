//
// Created by Saúl on 8/01/2026.
//

#include "Octree.h"

Octree::Octree() {
    root = nullptr;
}

Octree::~Octree() {
}

void Octree::insert() {
}

void Octree::erase() {
}

void Octree::create_space() {
    recursively_create_space(root);
}

void Octree::recursively_create_space(NodeOctree *root) {
}

void Octree::recursively_insert(NodeOctree *node_octree) {
}

void Octree::recursively_erase(NodeOctree *node_octree) {
}

