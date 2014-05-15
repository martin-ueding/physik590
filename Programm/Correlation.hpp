// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <boost/numeric/ublas/matrix.hpp>

#include <vector>

class Correlation {
    public:
        /**
          Calculates the correlation matrix for a single trajectory.

          @param[in] x Trajectory
          @param[in] size Size of the matrix
          @param[in] distance Distance to calculate
          @param[in] even Calculate the even elements.
          */
        static boost::numeric::ublas::matrix<double>
        correlation(std::vector<double> &x, const unsigned size,
                    const unsigned distance, const bool even);
};
