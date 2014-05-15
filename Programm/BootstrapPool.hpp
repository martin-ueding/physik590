// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Histogram.hpp"
#include "MetropolisDriver.hpp"

#include <boost/numeric/ublas/matrix.hpp>

#include <vector>
#include <map>

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
        BootstrapPool(MetropolisDriver &driver, unsigned iterations);

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

        /**
          Pool of even correlation matrix function summands.

          Those are maps from the distances to the correlation matrix with that
          given distance. The matrix only contrains correlations for \c i and
          \c j that are even.
          */
        std::vector<std::map<unsigned, boost::numeric::ublas::matrix<double>>> even;

        /**
          Same as \ref even, just for odd \c i and \c j.
          */
        std::vector<std::map<unsigned, boost::numeric::ublas::matrix<double>>> pool;

        std::vector<Histogram> histograms;
};
