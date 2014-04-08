// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HarmonicOscillator.hpp"
#include "Settings.hpp"

#include <iostream>

int main() {
    Settings s;
    HarmonicOscillator ho {s.time_step, s.mass, s.mu_squared};

    std::cout << "1 → 0: " << ho.action_difference(0.0, 1.0, 0.0, 0.0)
              << std::endl;
    std::cout << "0 → 1: " << ho.action_difference(0.0, 0.0, 1.0, 0.0)
              << std::endl;
}
