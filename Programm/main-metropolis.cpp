// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include "HarmonicOszillator.hpp"
#include "Histogram.hpp"
#include "Trajectory.hpp"

#include <iostream>

int main() {
    HarmonicOszillator ho;
    Trajectory t(50, ho);


    t.save_plot_file("trajectory-01-init.csv");
    std::cout << "Action after init: " << t.action() << std::endl;

    t.set_to_random(40.);
    t.save_plot_file("trajectory-02-random.csv");
    std::cout << "Action after random: " << t.action() << std::endl;

    t.iteration(1, 40.);
    t.save_plot_file("trajectory-03-iteration.csv");
    std::cout << "Action after iteration: " << t.action() << std::endl;

    for (int i = 0; i < 1000; i++) {
        t.iteration(5, 40.);
    }
    t.save_plot_file("trajectory-04-more_iterations.csv");
    std::cout << "Action after more iterations: " << t.action() << std::endl;

    Histogram histogram(200);

    for (int i = 0; i < 1000; i++) {
        t.iteration(5, 40.);
        t.binning_snapshot(histogram);
    }

    histogram.save("histogram-1000.csv");

    return 0;
}
