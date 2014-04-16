// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "ScalarComputable.hpp"

#include <cmath>
#include <cstddef>
#include <string>
#include <vector>

class Moment : public ScalarComputable {
    public:
        /**
          Creates a new moment calculator.

          @param[in] power Order of the moment
          */
        Moment(int power);

        void add_sample(BootstrapSample &sample);

        //static constexpr double pi {4 * std::atan(1)};
        //static constexpr double sigma_theory {1 / (0.59 * std::sqrt(2 * pi))};

        std::string get_name();

    protected:
        int power;
};
