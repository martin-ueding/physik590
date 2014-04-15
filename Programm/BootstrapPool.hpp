// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Trajectory.hpp"

#include "MetropolisDriver.hpp"

#include <vector>

class BootstrapPool {
    public:
        BootstrapPool(MetropolisDriver driver);

    protected:
        std::vector<Trajectory> pool;
};
