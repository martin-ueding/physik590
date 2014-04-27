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
#include <vector>
#include <map>

class ResultSet {
    public:
        ResultSet(BootstrapPool &pool, Settings &settings);

        std::vector<std::shared_ptr<ScalarComputable>> computables;
        std::map<unsigned int, std::shared_ptr<Correlation>> correlations;

        PositionDensity dens;

        void print_results();

        void operator()();

        void save_correlations(std::string outfilename);

    protected:
        ConcurrentCounter counter {1000};
        BootstrapPool &pool;
        std::mt19937 engine;

        void compute_using_pool();
        void add_correlation(unsigned int distance);
};
