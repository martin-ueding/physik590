// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Histogram.hpp"
#include "MetropolisDriver.hpp"

#include <boost/numeric/ublas/matrix.hpp>

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
          Gives the size of the pool.
          */
        size_t size() {
            return trajectories.size();
        }

        /**
          The original trajectories.
          */
        std::vector<std::vector<double>> trajectories;

        std::vector<boost::numeric::ublas::matrix<double>> even;
        std::vector<boost::numeric::ublas::matrix<double>> odd;

        std::vector<Histogram> histograms;
};
