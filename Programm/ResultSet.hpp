// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "ConcurrentCounter.hpp"
#include "Correlation.hpp"
#include "Moment.hpp"
#include "PositionDensity.hpp"
#include "ScalarComputable.hpp"

#include <memory>

class ResultSet {
    public:
        ResultSet(BootstrapPool &pool);

        std::vector<std::shared_ptr<ScalarComputable>> computables;
        PositionDensity dens { -5, 5, 1000};

        void print_results();

        void operator()();

    protected:
        ConcurrentCounter counter {1000};
        BootstrapPool &pool;
        std::mt19937 engine;

        void compute_using_pool();
};
