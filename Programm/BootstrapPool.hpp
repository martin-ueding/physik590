// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "ListQuantity.hpp"

#include "MetropolisDriver.hpp"

#include <vector>

class BootstrapPool {
    public:
        BootstrapPool(MetropolisDriver driver, size_t iterations);

        size_t size() {
            return pool.size();
        }

        ListQuantity& operator[](size_t i) {
            return pool[i];
        }

    protected:
        std::vector<ListQuantity> pool;
        size_t iterations;
};
