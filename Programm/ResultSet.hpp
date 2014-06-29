// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "ConcurrentCounter.hpp"
#include "Correlation.hpp"
#include "PositionDensity.hpp"
#include "ProgressBar.hpp"

#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include <memory>

class ResultSet {
    public:
        ResultSet(BootstrapPool &pool);

        PositionDensity dens;

        void print_results();

        void operator()();

        void save_correlations(std::string outfilename);

    protected:
        ConcurrentCounter counter {1000};
        BootstrapPool &pool;
        std::mt19937 engine;
};
