// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapSample.hpp"

#include <iostream>
#include <random>

BootstrapSample::BootstrapSample(BootstrapPool pool) {
    std::mt19937 engine;
    std::uniform_int_distribution<size_t> dist {0, pool.size()-1};
    
    // Fill the sample with as many elements as the pool has.
    for (size_t i {0}; i < pool.size(); ++i) {
        trajectories.push_back(&pool[dist(engine)]);
    }

    std::cout << "Bootstrap sample contains " << pool.size() << " elements now." << std::endl;

}
