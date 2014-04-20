// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "ConcurrentCounter.hpp"
#include "Correlation.hpp"
#include "Moment.hpp"
#include "PositionDensity.hpp"
#include "ScalarComputable.hpp"

class ResultSet {
    public:
        ResultSet(BootstrapPool &pool);

        Moment mean {1};
        Moment moment_2 {2};
        Correlation corr_10 {10};
        Correlation corr_100 {100};
        Correlation corr_999 {999};
        PositionDensity dens { -5, 5, 1000};

        std::vector<ScalarComputable *> computables;

        void print_results();

        void operator()();

    protected:
        ConcurrentCounter counter {1000};
        BootstrapPool &pool;
        std::mt19937 engine;

        void compute_using_pool();
};
