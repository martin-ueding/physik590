// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <eigen3/Eigen/Dense>

#include <vector>

class GEVPSolver {
    public:
        static std::vector<double> eigenvalues(Eigen::MatrixXd &a, Eigen::MatrixXd &b);
};
