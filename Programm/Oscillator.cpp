// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Oscillator.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <cmath>

Oscillator::Oscillator(double time_step, double mass,
                       double mu_squared, double gauss_height, double gauss_width) :
    Oscillator {time_step, mass},
           mu_squared {mu_squared},
           gauss_height {gauss_height},
           gauss_width {gauss_width},
gauss_width_squared {gauss_width * gauss_width} {
}

double Oscillator::action(std::vector<double> &x) {
    double sum {0.0};

    for (size_t i {0}; i < x.size() - 1; i++) {
        sum += action_step(x[i], x[i + 1]);
    }

    return sum;
}

double Oscillator::action_difference(double prev, double cur, double alt,
                                     double next) {
    double action_cur {action_step(prev, cur) + action_step(cur, next)};
    double action_alt {action_step(prev, alt) + action_step(alt, next)};

    return action_alt - action_cur;
}

double Oscillator::action_step(double cur, double next) {
    double difference {next - cur};
    double kinetic_part {
        0.5 * mass *(difference * difference) / (time_step * time_step)
    };
    double potential_part {potential(cur)};
    return time_step * (kinetic_part + potential_part);
}

void Oscillator::export_potential(std::string filename, std::string preamble) {
    std::ofstream handle {filename};
    handle << preamble;
    int steps = 1000;
    double bound {5.0};
    double step {bound / 2 / steps};
    for (double x { -bound}; x <= bound; x += step) {
        handle << x << "\t" << potential(x) << std::endl;
    }
}

double Oscillator::potential(double x) {
    return 0.5 * mu_squared * x * x
           + gauss_height * std::exp(-x * x / gauss_width_squared);
}
