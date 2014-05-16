// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "GEVPSolver.hpp"

#include <eigen3/Eigen/Eigenvalues>

std::vector<double> GEVPSolver::eigenvalues(Eigen::MatrixXd &a, Eigen::MatrixXd &b) {
    Eigen::MatrixXd l = a.llt().matrixL();
    Eigen::MatrixXd l_dagger = l.adjoint();
    Eigen::MatrixXd l_inverse = l.inverse();
    Eigen::MatrixXd l_dagger_inverse = l_dagger.inverse();

    Eigen::MatrixXd a_prime = l_inverse * a * l_dagger_inverse;

    Eigen::EigenSolver<Eigen::MatrixXd> ev_solver {a_prime, false};
    auto eigenvalues = ev_solver.eigenvalues();

    std::vector<double> eigenvalues_vector;

    for (auto &eigenvalue : eigenvalues) {
        eigenvalues_vector.push_back(eigenvalue);
    }

    return eigenvalues_vector;
}
