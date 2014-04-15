// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "Trajectory.hpp"

#include <vector>

class BootstrapSample {
    public:
        /**
          Creates a a new sample from the given pool.
          */
        BootstrapSample(BootstrapPool pool);

        /**
          Container with pointers to the trajectory in the pool.

          This assumes that the pool lives longer than the samples.
          Trajectories do not have to be copied, this should save a lot of
          memory.
          */
        std::vector<Trajectory*> trajectories;
};
