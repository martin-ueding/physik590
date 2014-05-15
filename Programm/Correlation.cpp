// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include "Periodic.hpp"

#include <cmath>
#include <iostream>

boost::numeric::ublas::matrix Correlation::correlation(std::vector<double> &x,
        const unsigned size, const unsigned distance, const bool even) {
    boost::numeric::ublas::matrix<double> c {size, size};

    std::cout << c(0, 0) << std::endl;
    c.clear();
    std::cout << c(0, 0) << std::endl;

    for (unsigned i {even ? 0 : 1}; i < c.size1; i += 2) {
        for (unsigned j {even ? 0 : 1}; j < c.size2; j += 2) {
            for (unsigned k {0}; k < x.size(); ++k) {
                c(i, j) += std::pow(x[k], i) * std::pow(x[Periodic::wrap(k + distance, x.size())], j);
            }
        }
    }

    // Normalize the elements.
    c /= x.size();

    return c;
}
