//
// Created by saul on 1/05/26.
//

#include "LinearNode.h"

LinearNode::LinearNode() {
}

LinearNode::~LinearNode() {
}

unsigned int LinearNode::get_morton_code() const {
    return mortonCode;
}

void LinearNode::set_morton_code(const unsigned int morton_code) {
    mortonCode = morton_code;
}
