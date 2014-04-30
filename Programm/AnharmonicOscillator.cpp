// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "AnharmonicOscillator.hpp"

#include <cmath>

AnharmonicOscillator::AnharmonicOscillator(double time_step, double mass,
        double mu_squared, double gauss_height, double gauss_width) :
        System {time_step, mass},
        mu_squared {mu_squared},
        gauss_height {gauss_height},
        gauss_width {gauss_width},
        gauss_width_squared {gauss_width * gauss_width} {
}

double AnharmonicOscillator::potential(double x) {
    return 0.5 * mu_squared * x * x
           + gauss_height * std::exp(-x * x / gauss_width_squared);
}

/*
double AnharmonicOscillator::ground_energy(double moment_2) {
    return std::pow(mu_squared, 2) * moment_2;
}
*/
