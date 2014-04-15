// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Moment.hpp"
#include "ScalarComputable.hpp"
#include "BootstrapPool.hpp"

class ResultSet {
    public:
        ResultSet(BootstrapPool pool);

        Moment mean {1};
        Moment moment_2 {2};
        std::vector<ScalarComputable *> computables;

        void print_results();

    protected:
        void compute_using_pool(BootstrapPool pool);
};
