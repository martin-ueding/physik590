// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "MetropolisAlgorithm.hpp"

#include "Periodic.hpp"

#include <cmath>

MetropolisAlgorithm::MetropolisAlgorithm(std::vector<double> &x, Oscillator &s,
        int position_seed, int accept_seed) :
    x(x), system(s),
    position_engine(std::mt19937 {position_seed}),
accept_engine(std::mt19937 {accept_seed}) {
}

bool MetropolisAlgorithm::accept_action_difference(double action_difference) {
    samples++;
    if (action_difference < 0) {
        accepted_negative++;
        return true;
    }
    else {
        bool rval;
        rval = std::exp(- action_difference) > zero_one_dist(accept_engine);
        if (rval) {
            accepted_exponential++;
        }
        return rval;
    }
}

void MetropolisAlgorithm::iteration(int rounds, double margin) {
    for (unsigned int j {0}; j < x.size(); j++) {
        // Wrap j around to create periodic boundary conditions.
        unsigned int j_plus_one {Periodic::wrap(j + 1, x.size())};
        unsigned int j_minus_one {Periodic::wrap(j - 1, x.size())};

        for (int round {0}; round < rounds; round++) {
            // Create a random number generator that will draw x_j' from a
            // normal distribution with mean = x_j and stddev = margin.
            std::normal_distribution<double> distribution {x[j], margin};

            // Generate a new random x_j'.
            double new_x {distribution(position_engine)};

            // Compute the difference in action (ΔS) that will result from the
            // exchange x_j → x_j'.
            double action_difference {
                system.action_difference(x[j_minus_one], x[j], new_x,
                x[j_plus_one])
            };

            if (accept_action_difference(action_difference)) {
                x[j] = new_x;
            }
        }
    }
}

void MetropolisAlgorithm::reset_accept_rate() {
    samples = 0;
    accepted_negative = 0;
    accepted_exponential = 0;
}

double MetropolisAlgorithm::get_accept_rate() {
    return (double)(accepted_negative + accepted_exponential) / samples;
}

double MetropolisAlgorithm::get_accept_rate_negative() {
    return (double) accepted_negative / samples;
}

double MetropolisAlgorithm::get_accept_rate_exponential() {
    return (double) accepted_exponential / samples;
}

void MetropolisAlgorithm::re_seed(int seed) {
    position_engine.seed(seed);
    accept_engine.seed(seed);
}
