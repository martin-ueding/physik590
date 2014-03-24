// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "HarmonicOszillator.hpp"
#include "Trajectory.hpp"

#include <iostream>

int main() {
    HarmonicOszillator ho;
    Trajectory t(10, &ho);

    t.print();
    t.set_to_random(40.);
    t.print();

    std::cout << "S: " << t.action() << std::endl;

    return 0;
}
