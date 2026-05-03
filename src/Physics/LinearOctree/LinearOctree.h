//
// Created by saul on 1/05/26.
//

#ifndef SIMULADORGRAVITACIONAL_LINEAROCTREE_H
#define SIMULADORGRAVITACIONAL_LINEAROCTREE_H
#include "LinearElement.h"
#include <vector>

template<typename T>
class LinearOctree {
    public:
    LinearOctree();
    virtual ~LinearOctree();
    void insert(T);
    private:
    std::vector<LinearElement<T>> elements{};

};

#endif //SIMULADORGRAVITACIONAL_LINEAROCTREE_H
