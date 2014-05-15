// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "MetropolisDriver.hpp"

#include <vector>

/**
  The base set of trajectories that are used for bootstrapping later on.
  */
class BootstrapPool {
    public:
        /**
          Fills the pool with trajectories.

          @param[in] driver Driver that generates trajectories
          @param[in] iterations Number of pool entries
          */
        BootstrapPool(MetropolisDriver &driver, size_t iterations);

        /**
          Gives the size of the pool
          */
        size_t size() {
            return pool.size();
        }

        /**
          Gives access to the elements in the pool.
          */
        ListQuantity &operator[](size_t i) {
            return pool[i];
        }

    protected:
        /**
          The pool itself.

          Each entry in the primary pool is just a trajectory from the
          Metropolis algorithm.
          */
        std::vector<std::vector<double>> pool;
};
