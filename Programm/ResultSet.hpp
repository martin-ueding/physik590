// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrapPool.hpp"
#include "Moment.hpp"
#include "PositionDensity.hpp"
#include "ScalarComputable.hpp"

class ResultSet {
    public:
        ResultSet(BootstrapPool pool);

        Moment mean {1};
        Moment moment_2 {2};
        PositionDensity dens {-5, 5, 1000};

        std::vector<ScalarComputable *> computables;

        void print_results();

    protected:
        void compute_using_pool(BootstrapPool pool);
};
