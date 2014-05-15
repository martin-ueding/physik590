// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

#include "Periodic.hpp"

#include <cmath>
#include <iostream>

boost::numeric::ublas::matrix Correlation::correlation(std::vector<double> &x, unsigned size, unsigned distance) {
    boost::numeric::ublas::matrix<double> c {size, size};

    std::cout << c(0, 0) << std::endl;
    c.clear();
    std::cout << c(0, 0) << std::endl;

    for (unsigned i {0}; i < c.size1; ++i) {
        for (unsigned j {0}; j < c.size2; ++j) {
            for (unsigned k {0}; k < x.size(); ++i) {
                c(i, j) += x[k] * x[Periodic::wrap(k + distance, x.size())];
            }
        }
    }

    // Normalize the elemnts 
    c /= x.size();

    return c;
}
