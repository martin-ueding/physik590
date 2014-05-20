// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include "Periodic.hpp"

#include <cmath>
#include <iostream>

Eigen::MatrixXd correlation(std::vector<double> &x, Settings &settings,
                         const unsigned distance, const bool even) {
    Eigen::MatrixXd c {settings.correlation_size, settings.correlation_size};
    c.setZero();

    for (unsigned row {0u} ; row < c.rows(); row++) {
        unsigned power1 {settings.matrix_to_state(row, even)};
        for (unsigned col {0u}; col < c.cols(); col++) {
            unsigned power2 {settings.matrix_to_state(col, even)};
            for (unsigned k {0u}; k < x.size(); ++k) {
                double x1 {x[k]};
                double x2 {x[Periodic::wrap(k + distance, x.size())]};
                c(row, col) += std::pow(x1, power1) * std::pow(x2, power2);
            }
        }
    }

    // Normalize the elements.
    c /= x.size();

    return c;
}
