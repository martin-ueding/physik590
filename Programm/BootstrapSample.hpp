// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"

#include <random>
#include <vector>

class BootstrapSample {
    public:
        /**
          Creates a a new sample from the given pool.
          */
        BootstrapSample(BootstrapPool &pool, std::mt19937 &engine);

        /**
          Combined correlation matrix function from all the summands.
          */
        std::map<unsigned, boost::numeric::ublas::matrix<double>> even;

        /**
          Similar to \ref even.
          */
        std::map<unsigned, boost::numeric::ublas::matrix<double>> odd;

        /**
          Histogram combined from all the summand histograms.
          */
        Histogram histogram;
};
