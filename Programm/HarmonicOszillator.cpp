// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HarmonicOszillator.hpp"

HarmonicOszillator::HarmonicOszillator(double time_step, double mass, double omega_squared) :
    System(time_step, mass), omega_squared(omega_squared) {
    }

double HarmonicOszillator::potential(double x) {
    return 0.5 * mass * omega_squared * x * x;
}
