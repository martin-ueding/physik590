// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "Settings.hpp"

#include <random>
#include <vector>

class BootstrapSample {
    public:
        /**
          Creates a a new sample from the given pool.
          */
        BootstrapSample(BootstrapPool &pool, Settings &settings);

        /**
          Combined correlation matrix function from all the summands.
          */
        CorrList even;

        /**
          Similar to \ref even.
          */
        CorrList odd;

        /**
          Histogram combined from all the summand histograms.
          */
        Histogram histogram;

        double e0_virial {0};
};
