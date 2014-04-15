// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "Trajectory.hpp"

#include <vector>

class BootstrapSample {
    public:
        BootstrapSample(BootstrapPool pool);

    protected:
        std::vector<Trajectory> trajectories;
};
