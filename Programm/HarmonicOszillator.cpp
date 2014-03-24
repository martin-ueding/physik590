// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HarmonicOszillator.hpp"

double HarmonicOszillator::action_step(double cur, double next) {
    double difference = next - cur;
    double difference_squared = difference * difference;
    return 0.5 * mass * difference_squared / a + a * potential(cur);
}
