//
// Created by Saúl on 8/01/2026.
//

#include "NodeOctree.h"

NodeOctree::NodeOctree() {

}

NodeOctree::~NodeOctree() {
    for (int i = 0; i < 8; i++) delete children[i];
}

