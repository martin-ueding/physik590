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
          */
        boost::numeric::ublas::matrix correlation(std::vector<double> &x, unsigned size, unsigned distance);
};
