// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapSample.hpp"

BootstrapSample::BootstrapSample(BootstrapPool &pool, std::mt19937 &engine) : pool(pool) {
    std::uniform_int_distribution<size_t> dist {0, pool.size() - 1};

    // Fill the sample with as many elements as the pool has.
    for (size_t i {0}; i < pool.size(); ++i) {
        indices.push_back(dist(engine));
    }
}

ListQuantity &BootstrapSample::operator[](size_t i) {
    return pool[indices[i]];
}

size_t BootstrapSample::size() {
    return indices.size();
}