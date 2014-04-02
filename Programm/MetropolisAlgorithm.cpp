// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "MetropolisAlgorithm.hpp"

#include "Periodic.hpp"

#include <cmath>
#include <functional>

MetropolisAlgorithm::MetropolisAlgorithm(ListQuantity &trajectory, System &s) :
    x(trajectory), system(s) {
}

bool MetropolisAlgorithm::accept_action_difference(double action_difference) {
    if (action_difference < 0) {
        return true;
    }

    return std::exp(- action_difference) > zero_one_dist(mt_engine);
}

void MetropolisAlgorithm::iteration(int rounds, double margin) {
    for (unsigned int j = 1; j < x.list.size(); j++) {
        std::uniform_real_distribution<double> distribution(x.list[j] - margin, x.list[j] + margin);

        // Wrap j around to create periodic boundary conditions.
        unsigned int j_plus_one = Periodic::wrap(j + 1, x.list.size());
        unsigned int j_minus_one = Periodic::wrap(j - 1, x.list.size());

        for (int round = 0; round < rounds; round++) {
            double new_x = distribution(mt_engine);

            double action_difference = system.action_difference(x.list[j_minus_one], x.list[j], new_x, x.list[j_plus_one]);

            if (accept_action_difference(action_difference)) {
                x.list[j] = new_x;
            }
        }
    }
}
