// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "MetropolisAlgorithm.hpp"

#include "Periodic.hpp"

#include <cmath>

MetropolisAlgorithm::MetropolisAlgorithm(ListQuantity &trajectory, System &s,
        int position_seed, int accept_seed) : x(trajectory), system(s),
    position_engine(std::mt19937 {position_seed}),
accept_engine(std::mt19937 {accept_seed}) {
}

bool MetropolisAlgorithm::accept_action_difference(double action_difference) {
    bool rval;
    if (action_difference < 0) {
        rval = true;
    }

    rval = std::exp(- action_difference) > zero_one_dist(accept_engine);

    samples++;
    if (rval) {
        accepted++;
    }

    return rval;
}

void MetropolisAlgorithm::iteration(int rounds, double margin) {
    for (unsigned int j {0}; j < x.list.size(); j++) {
        std::uniform_real_distribution<double> distribution {x.list[j] - margin, x.list[j] + margin};

        // Wrap j around to create periodic boundary conditions.
        unsigned int j_plus_one {Periodic::wrap(j + 1, x.list.size())};
        unsigned int j_minus_one {Periodic::wrap(j - 1, x.list.size())};

        for (int round {0}; round < rounds; round++) {
            double new_x {distribution(position_engine)};

            double action_difference {system.action_difference(x.list[j_minus_one], x.list[j], new_x, x.list[j_plus_one])};

            if (accept_action_difference(action_difference)) {
                x.list[j] = new_x;
            }
        }
    }
}

void MetropolisAlgorithm::reset_accept_rate() {
    samples = 0;
    accepted = 0;
}

double MetropolisAlgorithm::get_accept_rate() {
    return (double) accepted / samples;
}
