// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HarmonicOscillator.hpp"

#include <cmath>

HarmonicOscillator::HarmonicOscillator(double time_step, double mass,
                                       double mu_squared) : System(time_step, mass), mu_squared(mu_squared) {
}

double HarmonicOscillator::potential(double x) {
    return 0.5 * mu_squared * x * x;
}

double HarmonicOscillator::ground_energy(double moment_2) {
    return std::pow(mu_squared, 2) * moment_2;
}
