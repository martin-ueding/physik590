// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Oscillator.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <cmath>


Oscillator::Oscillator(Settings &settings) : settings {settings} {
    gauss_width_squared = settings.gauss_width * settings.gauss_width;
    double pi {std::atan(1) * 4};
    prefactor = 1. / std::sqrt(2. * pi * gauss_width_squared) * 2. * settings.inverse_scattering_length;
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
        0.5 * settings.mass *(difference * difference) / (settings.time_step * settings.time_step)
    };
    double potential_part {potential(cur)};
    return settings.time_step * (kinetic_part + potential_part);
}

void Oscillator::export_potential(std::string filename, std::string preamble,
                                  Settings &settings) {
    std::ofstream handle {filename};
    handle << preamble;
    double step {settings.export_potential_bound / 2 / settings.export_potential_steps};
    for (double x { -settings.export_potential_bound}; x <= settings.export_potential_bound; x += step) {
        handle << x << "\t" << potential(x) << std::endl;
    }
}

double Oscillator::potential(double x) {
    return 0.5 * settings.mu_squared * x * x
           + prefactor * std::exp(-x * x / (2 * gauss_width_squared));
}
