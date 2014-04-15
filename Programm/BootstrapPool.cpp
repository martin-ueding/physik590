// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include <iostream>

BootstrapPool::BootstrapPool(MetropolisDriver driver, size_t iterations)
    : iterations(iterations) {
    for (size_t i {0}; i < iterations; ++i) {
        pool.push_back(driver.next());
    }

    std::cout << "Bootstrap pool contains " << pool.size() << " elements now." << std::endl;
}
