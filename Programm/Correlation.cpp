// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include "Periodic.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

Eigen::MatrixXd correlation(std::vector<double> &x, Settings &settings,
                            const unsigned distance, const bool even) {
    Eigen::MatrixXd c {settings.correlation_size, settings.correlation_size};
    c.setZero();

    // Compute the powers of the trajectory so that the power function does not
    // need to be invoked that often. I have not tested, but I assume that this
    // is a hotspot.

    std::vector<std::vector<double>> powers(c.rows());

    for (unsigned row {0u}; row < c.rows(); row++) {
        unsigned power {settings.matrix_to_state(row, even)};
        assert(power > 0);

        // Bring the memory to the required size.
        powers[row].resize(x.size());

        // Take the power from all the x[k].
        for (unsigned k {0u}; k < x.size(); ++k) {
            powers[row][k] = std::pow(x[k], power);
        }
    }

    for (unsigned row {0u}; row < c.rows(); row++) {
        for (unsigned col {0u}; col < c.cols(); col++) {
            for (unsigned k {0u}; k < x.size(); ++k) {
                // Calculate the needed k's.
                unsigned k1 {k};
                unsigned k2 {Periodic::wrap(k + distance, x.size())};

                // Add x[k1]**power1 + x[k2]**power2 to the matrix element
                // using the precomputed powers.
                c(row, col) += powers[row][k1] * powers[col][k2];
            }
        }
    }

    // Normalize the elements.
    c /= x.size();

    return c;
}
