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
    Trajectory t(50, ho);

    t.save_plot_file("trajectory-01-init.csv");
    t.set_to_random(40.);
    t.save_plot_file("trajectory-02-random.csv");

    std::cout << "S: " << t.action() << std::endl;

    return 0;
}
