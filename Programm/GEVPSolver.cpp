// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "GEVPSolver.hpp"

#include <eigen3/Eigen/Eigenvalues>

#include <algorithm>
#include <iostream>

std::vector<double> GEVPSolver::eigenvalues(Eigen::MatrixXd &a,
                                            Eigen::MatrixXd &b) {
    Eigen::MatrixXd l = b.llt().matrixL();
    Eigen::MatrixXd l_dagger = l.adjoint();
    Eigen::MatrixXd l_inverse = l.inverse();
    Eigen::MatrixXd l_dagger_inverse = l_dagger.inverse();

    Eigen::MatrixXd a_prime = l_inverse * a * l_dagger_inverse;

    Eigen::EigenSolver<Eigen::MatrixXd> ev_solver{a_prime, false};
    auto eigenvalues = ev_solver.eigenvalues();

    std::vector<double> eigenvalues_vector;

    for (unsigned row_id{0u}; row_id < eigenvalues.rows(); row_id++) {
        std::complex<double> ev = eigenvalues(row_id, 0);
        eigenvalues_vector.push_back(ev.real());
    }

    // Reverse sort of the eigenvalues. Beware that those are reverse
    // iterators.
    sort(eigenvalues_vector.rbegin(), eigenvalues_vector.rend());

    return eigenvalues_vector;
}
