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

    for (unsigned i {even ? 0u : 1u}; i < c.rows(); i += 2) {
        for (unsigned j {even ? 0u : 1u}; j < c.cols(); j += 2) {
            for (unsigned k {0u}; k < x.size(); ++k) {
                c(i, j) += std::pow(x[k], i) * std::pow(x[Periodic::wrap(k + distance, x.size())], j);
            }
        }
    }

    // Normalize the elements.
    c /= x.size();

    return c;
}
