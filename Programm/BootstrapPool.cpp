// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "ProgressBar.hpp"

BootstrapPool::BootstrapPool(MetropolisDriver &driver, size_t iterations) {
    ProgressBar bar{"Populating bootstrap pool", iterations};
    for (size_t i {0}; i < iterations; ++i) {
        pool.push_back(driver.next());
        bar.update(i);
    }
}
