//
// Created by saul on 1/05/26.
//

#include "LinearOctree.h"
template<typename T>
LinearOctree<T>::LinearOctree() {
}

template<typename T>
LinearOctree<T>::~LinearOctree() {
    elements.clear();
}

template<typename T>
void LinearOctree<T>::insert(T) {
    LinearElement<T> node = new LinearElement<T>();

}