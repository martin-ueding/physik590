// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "ScalarComputable.hpp"

#include <cmath>
#include <cstddef>
#include <string>
#include <vector>

class Correlation : public ScalarComputable {
    public:
        /**
          Creates a new correlation calculator.

          @param[in] distance Lattice distance
          */
        Correlation(unsigned int distance);

        void add_sample(BootstrapSample &sample);

        std::string get_name();

    protected:
        unsigned int distance;

        /**
          Averages all correlations with a given distance in that list.

          Using periodic boundary conditions, there are as many correlations to
          be extracted as there are elements in the given list. All those are
          averaged.

          @param[in] list List with data points
          @param[in] distance Difference in steps
          */
        double average_all(std::vector<double> &list, const unsigned distance);
};
