// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapSample.hpp"

#include <iostream>
#include <random>

BootstrapSample::BootstrapSample(BootstrapPool pool) {
    std::mt19937 engine;
    std::uniform_int_distribution<size_t> dist {0, pool.size()-1};
    
    // TODO Move this number into settings.
    for (int i {0}; i < 1000; ++i) {
        trajectories.push_back(&pool[dist(engine)]);
    }

    std::cout << "Bootstrap sample contains " << pool.size() << " elements now." << std::endl;

}
