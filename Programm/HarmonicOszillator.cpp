// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HarmonicOszillator.hpp"

HarmonicOszillator::HarmonicOszillator(double time_step, double mass, double mu_squared) :
    System(time_step, mass), mu_squared(mu_squared) {
    }

double HarmonicOszillator::potential(double x) {
    return 0.5 * mu_squared * x * x;
}
