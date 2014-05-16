// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include "Periodic.hpp"

#include <cmath>
#include <iostream>

Eigen::MatrixXd
Correlation::correlation(std::vector<double> &x, const unsigned size,
        const unsigned distance, const bool even) {
    Eigen::MatrixXd c {size, size};
    c.setZero();

    for (unsigned row {0u} ; row < c.rows(); row++) {
        for (unsigned col {0u}; col < c.cols(); col++) {
            for (unsigned k {0u}; k < x.size(); ++k) {
                unsigned power1 {(even ? 0 : 1) + 2 * row};
                unsigned power2 {(even ? 0 : 1) + 2 * col};
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
